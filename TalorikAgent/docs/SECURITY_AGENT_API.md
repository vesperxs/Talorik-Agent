# TalorikAgent Security API

This document describes the REST API endpoints provided by the TalorikAgent Security Agent for integration with the React cybersecurity dashboard.

## Quick Start

1. **Build the project:**
   ```bash
   mkdir build && cd build
   cmake ..
   cmake --build .
   ```

2. **Run the security agent:**
   ```bash
   ./bin/TalorikAgent
   ```

3. **Test the API:**
   ```bash
   python scripts/test_api.py
   ```

## API Endpoints

The security agent runs on `http://localhost:8080` and provides the following endpoints:

### 1. Security Metrics
```http
GET /api/security/metrics
```

**Response:**
```json
{
  "totalThreats": 1247,
  "blockedAttacks": 1189,
  "activeAlerts": 23,
  "securityScore": 87.5,
  "uptime": 99.9,
  "lastScan": "2024-01-15T10:30:00Z"
}
```

### 2. Threat Data (Time Series)
```http
GET /api/threats/data?range=24h
```

**Parameters:**
- `range`: Time range for data (1h, 6h, 12h, 24h, 7d)

**Response:**
```json
[
  {
    "timestamp": "2024-01-15T10:00:00Z",
    "total_threats": 12,
    "blocked_threats": 11,
    "attack_types": ["ddos", "sql_injection"]
  }
]
```

### 3. Attack Types Distribution
```http
GET /api/threats/attack-types
```

**Response:**
```json
[
  {
    "attack_type": "ddos",
    "count": 35,
    "percentage": 35.0
  }
]
```

### 4. Recent Alerts
```http
GET /api/alerts/recent?limit=10
```

**Parameters:**
- `limit`: Number of alerts to return (default: 10)

**Response:**
```json
[
  {
    "id": 1,
    "severity": "critical",
    "description": "Multiple failed login attempts detected",
    "timestamp": "2024-01-15T10:28:00Z",
    "source_ip": "192.168.1.100",
    "source": "192.168.1.100"
  }
]
```

### 5. System Status
```http
GET /api/system/status
```

**Response:**
```json
[
  {
    "name": "Web Server",
    "status": "online",
    "uptime": "99.9%",
    "icon": "server"
  }
]
```

### 6. Agent Status
```http
GET /api/agent/status
```

**Response:**
```json
{
  "connected": true,
  "lastHeartbeat": "2024-01-15T10:30:00Z",
  "version": "1.2.3",
  "uptime": "72h 15m 30s"
}
```

### 7. Trigger Security Scan
```http
POST /api/security/scan
Content-Type: application/json

{
  "type": "full"
}
```

**Request Body:**
- `type`: Scan type ("full", "quick", "targeted")

**Response:**
```json
{
  "scanId": "scan_12345",
  "status": "started",
  "estimatedDuration": "5m"
}
```

## Testing

### Using curl
```bash
# Get security metrics
curl http://localhost:8080/api/security/metrics

# Get threat data for last 24 hours
curl http://localhost:8080/api/threats/data?range=24h

# Get recent alerts
curl http://localhost:8080/api/alerts/recent?limit=5

# Trigger a security scan
curl -X POST http://localhost:8080/api/security/scan \
  -H "Content-Type: application/json" \
  -d '{"type":"full"}'
```

### Using Python
```python
import requests

# Get security metrics
response = requests.get("http://localhost:8080/api/security/metrics")
metrics = response.json()
print(f"Security Score: {metrics['securityScore']}")

# Trigger scan
scan_data = {"type": "quick"}
response = requests.post("http://localhost:8080/api/security/scan", json=scan_data)
scan_result = response.json()
print(f"Scan ID: {scan_result['scanId']}")
```

## React Dashboard Integration

### Environment Variables
Create a `.env` file in your React project:
```env
REACT_APP_API_URL=http://localhost:8080/api
REACT_APP_WS_URL=ws://localhost:8080/ws
```

### Example React Component
```jsx
import React, { useState, useEffect } from 'react';

function SecurityDashboard() {
  const [metrics, setMetrics] = useState(null);
  const [alerts, setAlerts] = useState([]);

  useEffect(() => {
    // Fetch security metrics
    fetch(`${process.env.REACT_APP_API_URL}/security/metrics`)
      .then(res => res.json())
      .then(data => setMetrics(data));

    // Fetch recent alerts
    fetch(`${process.env.REACT_APP_API_URL}/alerts/recent?limit=10`)
      .then(res => res.json())
      .then(data => setAlerts(data));
  }, []);

  return (
    <div>
      {metrics && (
        <div>
          <h2>Security Score: {metrics.securityScore}</h2>
          <p>Total Threats: {metrics.totalThreats}</p>
          <p>Blocked Attacks: {metrics.blockedAttacks}</p>
        </div>
      )}
      
      <h3>Recent Alerts</h3>
      {alerts.map(alert => (
        <div key={alert.id} className={`alert alert-${alert.severity}`}>
          {alert.description}
        </div>
      ))}
    </div>
  );
}
```

## Data Flow

1. **C++ Agent** continuously collects security data (simulated for demo)
2. **Agent** exposes data via REST API endpoints on port 8080
3. **React Dashboard** fetches data via HTTP requests
4. **Real-time updates** can be implemented via WebSocket (endpoint ready)
5. **Dashboard** transforms and displays data in charts/tables

## Configuration

The agent can be configured via `resources/config/agent_config.json`:

```json
{
  "api": {
    "port": 8080,
    "host": "0.0.0.0"
  },
  "security": {
    "dataCollectionInterval": 30,
    "maxThreatHistory": 1000,
    "maxAlerts": 100
  },
  "logging": {
    "level": "info",
    "file": "logs/security_agent.log"
  }
}
```

## Troubleshooting

### Common Issues

1. **Port 8080 already in use:**
   ```bash
   # Find process using port 8080
   netstat -ano | findstr :8080
   # Kill the process
   taskkill /PID <process_id> /F
   ```

2. **CORS errors in browser:**
   - The agent includes CORS middleware
   - Ensure your React app is making requests to the correct URL

3. **Build errors:**
   - Ensure vcpkg is properly configured
   - Install Crow dependency: `vcpkg install crow`

### Debug Mode

Enable debug logging by setting the log level to "debug" in the configuration file.

## Security Considerations

- The current implementation is for demonstration purposes
- In production, implement:
  - Authentication for API endpoints
  - HTTPS/TLS encryption
  - Input validation and sanitization
  - Rate limiting
  - Audit logging

## Development

### Adding New Endpoints

1. Add the endpoint handler in `SecurityAgent::setupApiRoutes()`
2. Implement the corresponding data method
3. Update the test script
4. Update this documentation

### Extending Data Models

1. Add new structures to `include/models/SecurityModels.h`
2. Implement JSON serialization in `src/models/SecurityModels.cpp`
3. Update the agent to use the new models 