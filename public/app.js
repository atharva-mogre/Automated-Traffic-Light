// Initialize Socket.IO connection
const socket = io();

// DOM Elements
const redLight = document.getElementById('red-light');
const yellowLight = document.getElementById('yellow-light');
const greenLight = document.getElementById('green-light');
const stateDisplay = document.getElementById('state-display');
const distanceValue = document.getElementById('distance-value');
const distanceBar = document.getElementById('distance-bar');
const lastUpdate = document.getElementById('last-update');
const connectionStatus = document.getElementById('connection-status');
const statusText = document.getElementById('status-text');

// Connection status handlers
socket.on('connect', () => {
    console.log('✅ Connected to server');
    updateConnectionStatus(true);
});

socket.on('disconnect', () => {
    console.log('❌ Disconnected from server');
    updateConnectionStatus(false);
});

socket.on('connect_error', (error) => {
    console.error('Connection error:', error);
    updateConnectionStatus(false);
});

// Listen for sensor data updates
socket.on('sensor-data', (data) => {
    console.log('📊 Received data:', data);

    // Validate data structure
    if (!data || typeof data !== 'object') {
        console.warn('⚠️ Invalid data received:', data);
        return;
    }

    // Check for required fields
    if (data.state === undefined || data.distance === undefined) {
        console.warn('⚠️ Missing required fields in data:', data);
        return;
    }

    // Log specific values for debugging
    console.log(`   └─ Distance: ${data.distance}cm, State: ${data.state}`);

    updateTrafficLight(data.state);
    updateDistance(data.distance);
    updateLastUpdateTime();
});

/**
 * Update connection status display
 */
function updateConnectionStatus(isConnected) {
    if (isConnected) {
        connectionStatus.classList.remove('disconnected');
        connectionStatus.classList.add('connected');
        statusText.textContent = 'Connected';
    } else {
        connectionStatus.classList.remove('connected');
        connectionStatus.classList.add('disconnected');
        statusText.textContent = 'Disconnected';
    }
}

/**
 * Update traffic light display based on state
 */
function updateTrafficLight(state) {
    // Remove active class from all lights
    redLight.classList.remove('active');
    yellowLight.classList.remove('active');
    greenLight.classList.remove('active');

    // Remove state classes from state display
    stateDisplay.classList.remove('RED', 'YELLOW', 'GREEN');

    // Activate appropriate light
    switch (state) {
        case 'RED':
            redLight.classList.add('active');
            stateDisplay.textContent = 'RED';
            stateDisplay.classList.add('RED');
            break;
        case 'YELLOW':
            yellowLight.classList.add('active');
            stateDisplay.textContent = 'YELLOW';
            stateDisplay.classList.add('YELLOW');
            break;
        case 'GREEN':
            greenLight.classList.add('active');
            stateDisplay.textContent = 'GREEN';
            stateDisplay.classList.add('GREEN');
            break;
        default:
            greenLight.classList.add('active');
            stateDisplay.textContent = 'UNKNOWN';
    }
}

/**
 * Update distance display and bar
 */
function updateDistance(distance) {
    // Ensure distance is a valid number
    const validDistance = parseFloat(distance) || 0;

    // Update distance value with animation
    const currentDistance = parseFloat(distanceValue.textContent) || 0;
    animateValue(distanceValue, currentDistance, validDistance, 300);

    // Update distance bar
    // Map distance to percentage (0-200cm range)
    const maxDistance = 200;
    let percentage = Math.min((validDistance / maxDistance) * 100, 100);

    distanceBar.style.width = percentage + '%';

    // Update bar color based on NEW logic (near = green, far = red)
    distanceBar.classList.remove('red-range', 'yellow-range', 'green-range');

    if (validDistance <= 15) {
        distanceBar.classList.add('green-range');  // Near = GREEN
    } else if (validDistance <= 30) {
        distanceBar.classList.add('yellow-range'); // Transition = YELLOW
    } else {
        distanceBar.classList.add('red-range');     // Far = RED
    }

    // Add visual pulse effect
    addPulseEffect();
}

/**
 * Animate number value change
 */
function animateValue(element, start, end, duration) {
    const range = end - start;
    const increment = range / (duration / 16); // 60fps
    let current = start;

    const timer = setInterval(() => {
        current += increment;

        if ((increment > 0 && current >= end) || (increment < 0 && current <= end)) {
            current = end;
            clearInterval(timer);
        }

        // Display with one decimal place for precision
        element.textContent = current.toFixed(1);
    }, 16);
}

/**
 * Update last update timestamp
 */
function updateLastUpdateTime() {
    const now = new Date();
    const timeString = now.toLocaleTimeString('en-US', {
        hour: '2-digit',
        minute: '2-digit',
        second: '2-digit',
        hour12: true
    });
    lastUpdate.textContent = timeString;
}

/**
 * Format time ago
 */
function timeAgo(date) {
    const seconds = Math.floor((new Date() - date) / 1000);

    if (seconds < 60) return 'Just now';
    if (seconds < 120) return '1 minute ago';
    if (seconds < 3600) return Math.floor(seconds / 60) + ' minutes ago';
    if (seconds < 7200) return '1 hour ago';
    return Math.floor(seconds / 3600) + ' hours ago';
}

/**
 * Add pulse effect to distance display on update
 */
function addPulseEffect() {
    const distanceDisplay = document.querySelector('.distance-display');
    if (distanceDisplay) {
        distanceDisplay.classList.remove('pulse-animation');
        // Force reflow to restart animation
        void distanceDisplay.offsetWidth;
        distanceDisplay.classList.add('pulse-animation');
    }
}

// Initialize with default state
console.log('🚦 Traffic Light Detector initialized');
console.log('⏳ Waiting for sensor data...');
