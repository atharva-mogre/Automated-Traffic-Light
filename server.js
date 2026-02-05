const express = require('express');
const http = require('http');
const socketIo = require('socket.io');
const { SerialPort } = require('serialport');
const { ReadlineParser } = require('@serialport/parser-readline');
const cors = require('cors');
const path = require('path');

// Initialize Express app
const app = express();
const server = http.createServer(app);
const io = socketIo(server, {
  cors: {
    origin: "*",
    methods: ["GET", "POST"]
  }
});

// Middleware
app.use(cors());
app.use(express.json());
app.use(express.static('public'));

// Store current traffic state
let currentState = {
  distance: 0,
  state: 'GREEN',
  timestamp: new Date().toISOString()
};

// Configure Serial Port
// IMPORTANT: Update the COM port to match your Arduino's port
// Windows: 'COM3', 'COM4', etc.
// Mac/Linux: '/dev/ttyUSB0', '/dev/ttyACM0', etc.
const PORT_NAME = 'COM12'; // Change this to your Arduino's port
const BAUD_RATE = 9600;

let serialPort;
let parser;

// Function to initialize serial connection
function initializeSerial() {
  try {
    serialPort = new SerialPort({
      path: PORT_NAME,
      baudRate: BAUD_RATE,
    });

    parser = serialPort.pipe(new ReadlineParser({ delimiter: '\n' }));

    serialPort.on('open', () => {
      console.log(`✅ Serial Port opened on ${PORT_NAME} at ${BAUD_RATE} baud`);
      console.log('Connected to Arduino - Receiving data...');
    });

    serialPort.on('error', (err) => {
      console.error('❌ Serial Port Error:', err.message);
      console.log('\n⚠️  TROUBLESHOOTING:');
      console.log(`1. Check if Arduino is connected to ${PORT_NAME}`);
      console.log('2. Update PORT_NAME in server.js to match your Arduino port');
      console.log('3. Close Arduino IDE Serial Monitor if open');
      console.log('4. Try unplugging and replugging the Arduino\n');
    });

    // Parse incoming data from Arduino
    parser.on('data', (data) => {
      try {
        const trimmedData = data.trim();

        // Only process JSON data
        if (trimmedData.startsWith('{')) {
          const sensorData = JSON.parse(trimmedData);

          // Update current state
          currentState = {
            distance: sensorData.distance,
            state: sensorData.state,
            timestamp: new Date().toISOString()
          };

          // Broadcast to all connected clients
          io.emit('sensor-data', currentState);

          // Log to console
          console.log(`📊 Distance: ${sensorData.distance}cm | State: ${sensorData.state}`);
        }
      } catch (err) {
        // Ignore non-JSON data (initialization messages, etc.)
        if (!data.includes('Traffic Light System Initialized') &&
          !data.includes('Monitoring traffic')) {
          console.log('📝 Arduino:', data.trim());
        }
      }
    });

  } catch (err) {
    console.error('❌ Failed to initialize serial port:', err.message);
    console.log('\n⚠️  Please check your Arduino connection and update PORT_NAME in server.js\n');
  }
}

// Initialize serial connection
initializeSerial();

// Socket.IO connection handling
io.on('connection', (socket) => {
  console.log('🔌 New client connected:', socket.id);

  // Send current state to newly connected client
  socket.emit('sensor-data', currentState);

  socket.on('disconnect', () => {
    console.log('🔌 Client disconnected:', socket.id);
  });
});

// REST API Endpoints

// Get current traffic state
app.get('/api/status', (req, res) => {
  res.json({
    success: true,
    data: currentState
  });
});

// Get system health
app.get('/api/health', (req, res) => {
  res.json({
    success: true,
    status: 'running',
    serialPort: serialPort && serialPort.isOpen ? 'connected' : 'disconnected',
    timestamp: new Date().toISOString()
  });
});

// List available serial ports (helpful for debugging)
app.get('/api/ports', async (req, res) => {
  try {
    const ports = await SerialPort.list();
    res.json({
      success: true,
      ports: ports.map(p => ({
        path: p.path,
        manufacturer: p.manufacturer,
        serialNumber: p.serialNumber
      }))
    });
  } catch (err) {
    res.status(500).json({
      success: false,
      error: err.message
    });
  }
});

// Serve index.html for root route
app.get('/', (req, res) => {
  res.sendFile(path.join(__dirname, 'public', 'index.html'));
});

// Start server
const PORT = 3000;
server.listen(PORT, () => {
  console.log('\n' + '='.repeat(60));
  console.log('🚦 Traffic Light Detector Server Started');
  console.log('='.repeat(60));
  console.log(`🌐 Web Interface: http://localhost:${PORT}`);
  console.log(`📡 API Endpoint: http://localhost:${PORT}/api/status`);
  console.log(`🔧 Port List: http://localhost:${PORT}/api/ports`);
  console.log('='.repeat(60) + '\n');
});

// Graceful shutdown
process.on('SIGINT', () => {
  console.log('\n\n🛑 Shutting down server...');
  if (serialPort && serialPort.isOpen) {
    serialPort.close(() => {
      console.log('✅ Serial port closed');
      process.exit(0);
    });
  } else {
    process.exit(0);
  }
});
