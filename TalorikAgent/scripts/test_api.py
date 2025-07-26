#!/usr/bin/env python3
"""
Test script for TalorikAgent Security API endpoints
"""

import requests
import json
import time
from datetime import datetime

BASE_URL = "http://localhost:8080/api"

def test_endpoint(endpoint, method="GET", data=None):
    """Test an API endpoint and return the response"""
    url = f"{BASE_URL}{endpoint}"
    
    try:
        if method == "GET":
            response = requests.get(url, timeout=10)
        elif method == "POST":
            response = requests.post(url, json=data, timeout=10)
        else:
            print(f"❌ Unsupported method: {method}")
            return None
            
        if response.status_code == 200:
            print(f"✅ {method} {endpoint} - Status: {response.status_code}")
            return response.json()
        else:
            print(f"❌ {method} {endpoint} - Status: {response.status_code}")
            print(f"   Response: {response.text}")
            return None
            
    except requests.exceptions.ConnectionError:
        print(f"❌ {method} {endpoint} - Connection refused (server not running?)")
        return None
    except Exception as e:
        print(f"❌ {method} {endpoint} - Error: {e}")
        return None

def main():
    print("🔍 Testing TalorikAgent Security API Endpoints")
    print("=" * 50)
    
    # Test 1: Security Metrics
    print("\n1. Testing Security Metrics")
    metrics = test_endpoint("/security/metrics")
    if metrics:
        print(f"   Total Threats: {metrics.get('totalThreats', 'N/A')}")
        print(f"   Blocked Attacks: {metrics.get('blockedAttacks', 'N/A')}")
        print(f"   Active Alerts: {metrics.get('activeAlerts', 'N/A')}")
        print(f"   Security Score: {metrics.get('securityScore', 'N/A')}")
    
    # Test 2: Threat Data
    print("\n2. Testing Threat Data (24h)")
    threat_data = test_endpoint("/threats/data?range=24h")
    if threat_data:
        print(f"   Data points: {len(threat_data)}")
        if threat_data:
            latest = threat_data[-1]
            print(f"   Latest timestamp: {latest.get('timestamp', 'N/A')}")
            print(f"   Latest threats: {latest.get('total_threats', 'N/A')}")
    
    # Test 3: Attack Types
    print("\n3. Testing Attack Types Distribution")
    attack_types = test_endpoint("/threats/attack-types")
    if attack_types:
        print(f"   Attack types: {len(attack_types)}")
        for attack in attack_types[:3]:  # Show first 3
            print(f"   - {attack.get('attack_type', 'N/A')}: {attack.get('count', 'N/A')} ({attack.get('percentage', 'N/A')}%)")
    
    # Test 4: Recent Alerts
    print("\n4. Testing Recent Alerts")
    alerts = test_endpoint("/alerts/recent?limit=5")
    if alerts:
        print(f"   Alerts: {len(alerts)}")
        for alert in alerts[:2]:  # Show first 2
            print(f"   - {alert.get('severity', 'N/A')}: {alert.get('description', 'N/A')}")
    
    # Test 5: System Status
    print("\n5. Testing System Status")
    system_status = test_endpoint("/system/status")
    if system_status:
        print(f"   Systems: {len(system_status)}")
        for system in system_status:
            print(f"   - {system.get('name', 'N/A')}: {system.get('status', 'N/A')} ({system.get('uptime', 'N/A')})")
    
    # Test 6: Agent Status
    print("\n6. Testing Agent Status")
    agent_status = test_endpoint("/agent/status")
    if agent_status:
        print(f"   Connected: {agent_status.get('connected', 'N/A')}")
        print(f"   Version: {agent_status.get('version', 'N/A')}")
        print(f"   Uptime: {agent_status.get('uptime', 'N/A')}")
    
    # Test 7: Security Scan
    print("\n7. Testing Security Scan")
    scan_data = {"type": "quick"}
    scan_response = test_endpoint("/security/scan", method="POST", data=scan_data)
    if scan_response:
        print(f"   Scan ID: {scan_response.get('scanId', 'N/A')}")
        print(f"   Status: {scan_response.get('status', 'N/A')}")
        print(f"   Duration: {scan_response.get('estimatedDuration', 'N/A')}")
    
    print("\n" + "=" * 50)
    print("🎉 API Testing Complete!")
    print("\nTo test with curl:")
    print("  curl http://localhost:8080/api/security/metrics")
    print("  curl http://localhost:8080/api/threats/data?range=24h")
    print("  curl http://localhost:8080/api/alerts/recent?limit=5")
    print("  curl -X POST http://localhost:8080/api/security/scan -H 'Content-Type: application/json' -d '{\"type\":\"full\"}'")

if __name__ == "__main__":
    main() 