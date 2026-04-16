import React, { useState, useEffect } from 'react';
import { ShieldAlert, AlertTriangle, Video, MapPin, Database } from 'lucide-react';
import { io } from 'socket.io-client';

const SentinelWatch = () => {
  const [detections, setDetections] = useState([]);
  const [activeAlert, setActiveAlert] = useState(false);
  const [isConnected, setIsConnected] = useState(false);

  useEffect(() => {
    const socket = io('http://localhost:3000');
    
    socket.on('connect', () => setIsConnected(true));
    socket.on('disconnect', () => setIsConnected(false));
    
    let alertTimeout;
    socket.on('defense_data', (newDetection) => {
       setActiveAlert(true);
       setDetections(prev => [newDetection, ...prev].slice(0, 15));
       clearTimeout(alertTimeout);
       alertTimeout = setTimeout(() => setActiveAlert(false), 2000);
    });

    return () => {
        socket.disconnect();
        clearTimeout(alertTimeout);
    };
  }, []);

  return (
    <div className="animate-slide-in flex-col" style={{ height: '100%', gap: '1.5rem' }}>
      <div className="flex justify-between items-center mb-4">
        <div>
          <h2 className="text-3xl font-bold text-red flex items-center gap-2">
            <ShieldAlert /> SENTINEL WATCH
          </h2>
          <p className="text-secondary text-mono mt-2">YOLOv8 DRONE INTRUSION DETECTION</p>
        </div>
        <div className={`btn ${activeAlert ? 'btn-danger' : ''}`} style={{ borderColor: activeAlert ? '' : 'var(--color-border)', cursor: 'default' }}>
          <AlertTriangle size={18} className={activeAlert ? 'text-red' : 'text-secondary'} />
          <span className={activeAlert ? 'text-red' : 'text-secondary'} >{activeAlert ? 'INTRUDER DETECTED' : 'AERIAL SPACE CLEAR'}</span>
        </div>
      </div>

      <div className="grid grid-cols-3 gap-6" style={{ height: '400px' }}>
        {/* Live Camera Feed Mock */}
        <div className="glass-panel hud-wrapper flex-col" style={{ gridColumn: 'span 2' }}>
          <div className="hud-corner hud-corner-red hud-tl"></div>
          <div className="hud-corner hud-corner-red hud-tr"></div>
          <div className="hud-corner hud-corner-red hud-bl"></div>
          <div className="hud-corner hud-corner-red hud-br"></div>
          <h3 className="text-xl mb-4 flex items-center gap-2 text-red">
            <Video size={20} /> THERMAL / OPTICAL FEED
          </h3>
          <div style={{ flex: 1, background: '#000', borderRadius: '8px', position: 'relative', border: '1px solid var(--color-border)', overflow: 'hidden' }}>
            <div style={{ position: 'absolute', top: '10px', right: '10px', color: 'red', fontSize: '0.8rem', fontFamily: 'monospace' }}>
              REC <span className="status-indicator status-alert" style={{ marginLeft: '5px', width: '8px', height: '8px' }}></span>
            </div>
            
            {/* Mock YOLO Bounding Box */}
            {activeAlert && (
              <div style={{
                position: 'absolute',
                top: '20%',
                left: '40%',
                width: '120px',
                height: '80px',
                border: '2px solid red',
                boxShadow: '0 0 10px rgba(239, 68, 68, 0.8)',
                animation: 'pulse-red 1s infinite'
              }}>
                <div style={{ background: 'red', color: 'white', fontSize: '0.7rem', padding: '2px 4px', position: 'absolute', top: '-18px', left: '-2px' }}>
                  DRONE 92%
                </div>
                {/* Crosshair targeting */}
                <div style={{ position: 'absolute', top: '50%', left: '50%', width: '10px', height: '10px', border: '1px solid red', transform: 'translate(-50%, -50%)', borderRadius: '50%' }}></div>
              </div>
            )}
            
            <div className="flex items-center justify-center p-4 h-full" style={{ color: 'var(--color-text-secondary)', fontFamily: 'var(--font-mono)' }}>
              YOLOv8 STREAM WAITING...
            </div>
          </div>
        </div>

        {/* GPS Radar */}
        <div className="glass-panel flex-col items-center justify-center">
          <h3 className="text-xl mb-6 flex items-center gap-2 self-start text-red">
            <MapPin size={20} /> GPS RADAR
          </h3>
          <div className="radar" style={{ width: '80%', maxWidth: '200px' }}>
            {activeAlert && (
              <div className="drone-dot" style={{ top: '30%', left: '60%' }}></div>
            )}
            {!activeAlert && detections.length > 0 && (
              <div className="drone-dot" style={{ top: '45%', left: '40%', opacity: 0.5, background: 'orange', boxShadow: 'none' }}></div>
            )}
          </div>
          <div className="text-mono mt-6 text-center text-secondary" style={{ fontSize: '0.8rem' }}>
            SCANNING SECTOR 7G <br/>
            MODEL: improved_gps_model.pkl
          </div>
        </div>
      </div>

      {/* Detection Logs */}
      <div className="glass-panel" style={{ flex: 1, overflowY: 'auto' }}>
        <h3 className="text-xl mb-4 flex items-center gap-2"><Database size={20}/> THREAT DETECTION LOGS</h3>
        <div className="text-mono" style={{ fontSize: '0.9rem' }}>
          <table style={{ width: '100%', textAlign: 'left', borderCollapse: 'collapse' }}>
            <thead>
              <tr style={{ borderBottom: '1px solid var(--color-border)', color: 'var(--color-text-secondary)' }}>
                <th className="p-4" style={{ padding: '0.5rem' }}>TIME</th>
                <th className="p-4" style={{ padding: '0.5rem' }}>LATITUDE</th>
                <th className="p-4" style={{ padding: '0.5rem' }}>LONGITUDE</th>
                <th className="p-4" style={{ padding: '0.5rem' }}>ALTITUDE (m)</th>
                <th className="p-4" style={{ padding: '0.5rem' }}>CONFIDENCE</th>
              </tr>
            </thead>
            <tbody>
              {detections.map((det, index) => (
                <tr key={det.id} style={{ borderBottom: '1px solid rgba(51, 65, 85, 0.5)', background: index === 0 && activeAlert ? 'rgba(239, 68, 68, 0.1)' : 'transparent' }}>
                  <td className="p-4" style={{ padding: '0.5rem' }}>{det.time}</td>
                  <td className="p-4" style={{ padding: '0.5rem' }}>{det.lat}</td>
                  <td className="p-4" style={{ padding: '0.5rem' }}>{det.lon}</td>
                  <td className="p-4" style={{ padding: '0.5rem' }}>{det.alt}</td>
                  <td className="p-4 text-red font-bold" style={{ padding: '0.5rem' }}>{det.conf}%</td>
                </tr>
              ))}
              {detections.length === 0 && (
                <tr><td colSpan="5" className="p-4 text-center text-secondary">No threats detected.</td></tr>
              )}
            </tbody>
          </table>
        </div>
      </div>
    </div>
  );
};

export default SentinelWatch;
