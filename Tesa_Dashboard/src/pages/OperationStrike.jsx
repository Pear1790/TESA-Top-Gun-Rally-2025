import React, { useState, useEffect } from 'react';
import { Crosshair, Activity, Cpu, Wind, Navigation } from 'lucide-react';
import { io } from 'socket.io-client';

const OperationStrike = () => {
  const [position, setPosition] = useState({ x: 0.0, y: 0.0, z: 2.5 });
  const [attitude, setAttitude] = useState({ roll: 0.0, pitch: 0.0, yaw: 45.0 });
  const [velocity, setVelocity] = useState({ x: 1.2, y: 0.8, z: 0.1 });
  const [thrust, setThrust] = useState(65);
  const [isConnected, setIsConnected] = useState(false);

  useEffect(() => {
    const socket = io('http://localhost:3000');
    
    socket.on('connect', () => setIsConnected(true));
    socket.on('disconnect', () => setIsConnected(false));
    
    socket.on('offense_data', (data) => {
      if(data) {
        setPosition({ x: data.x || 0, y: data.y || 0, z: data.z || 0 });
        setAttitude({ 
          roll: (data.roll || 0) * (180/Math.PI), 
          pitch: (data.pitch || 0) * (180/Math.PI), 
          yaw: (data.yaw || 0) * (180/Math.PI) 
        });
        setVelocity({ x: data.vx || 0, y: data.vy || 0, z: 0 });
        setThrust(data.thrust || 0);
      }
    });

    return () => socket.disconnect();
  }, []);

  return (
    <div className="animate-slide-in flex-col" style={{ height: '100%', gap: '1.5rem' }}>
      <div className="flex justify-between items-center mb-4">
        <div>
          <h2 className="text-3xl font-bold text-green flex items-center gap-2">
            <Crosshair /> OPERATION STRIKE
          </h2>
          <p className="text-secondary text-mono mt-2">QUADROTOR ATTACK TRAJECTORY CONTROL</p>
        </div>
        <button className="btn btn-primary">
          <Activity size={18} /> INITIATE TAKEOFF
        </button>
      </div>

      <div className="grid grid-cols-3 gap-6">
        {/* Spatial Coordinates */}
        <div className="glass-panel hud-wrapper">
          <div className="hud-corner hud-tl"></div>
          <div className="hud-corner hud-tr"></div>
          <div className="hud-corner hud-bl"></div>
          <div className="hud-corner hud-br"></div>
          
          <h3 className="text-xl mb-4 flex items-center gap-2 text-green">
            <Navigation size={20} /> SPATIAL POSITION
          </h3>
          <div className="text-mono" style={{ fontSize: '1.25rem', lineHeight: '2' }}>
            <div className="flex justify-between"><span>X-AXIS:</span> <span className="text-green">{position.x.toFixed(2)} m</span></div>
            <div className="flex justify-between"><span>Y-AXIS:</span> <span className="text-green">{position.y.toFixed(2)} m</span></div>
            <div className="flex justify-between"><span>ALTITUDE (Z):</span> <span className="text-green font-bold">{position.z.toFixed(2)} m</span></div>
          </div>
        </div>

        {/* Attitude Display */}
        <div className="glass-panel" style={{ borderColor: 'var(--color-accent-green)' }}>
          <h3 className="text-xl mb-4 flex items-center gap-2"><Cpu size={20}/> GYRO ATTITUDE</h3>
          <div className="text-mono" style={{ fontSize: '1.25rem', lineHeight: '2' }}>
            <div className="flex justify-between"><span>ROLL:</span> <span>{attitude.roll.toFixed(1)}°</span></div>
            <div className="flex justify-between"><span>PITCH:</span> <span>{attitude.pitch.toFixed(1)}°</span></div>
            <div className="flex justify-between"><span>YAW:</span> <span>{attitude.yaw.toFixed(1)}°</span></div>
          </div>
          <div className="mt-4 pt-4 border-t" style={{ borderColor: 'var(--color-border)', borderTop: '1px solid var(--color-border)' }}>
            <p className="text-secondary" style={{ fontSize: '0.85rem' }}>PD CONTROLLER: <span className="text-green">ONLINE</span></p>
          </div>
        </div>

        {/* Dynamics Panel */}
        <div className="glass-panel">
          <h3 className="text-xl mb-4 flex items-center gap-2"><Wind size={20}/> PROPULSION</h3>
          <div className="text-mono mb-6">
            <div className="flex justify-between mb-2">
              <span>Main Thrust</span>
              <span className="text-green">{thrust.toFixed(1)} %</span>
            </div>
            <div style={{ width: '100%', background: 'rgba(255,255,255,0.1)', height: '10px', borderRadius: '5px', overflow: 'hidden' }}>
              <div style={{ width: `${thrust}%`, background: 'var(--color-accent-green)', height: '100%', transition: 'width 0.3s ease' }}></div>
            </div>
          </div>
          <div className="text-mono" style={{ fontSize: '1rem', lineHeight: '1.8' }}>
            <div className="flex justify-between"><span>VELOCITY X:</span> <span>{velocity.x.toFixed(2)} m/s</span></div>
            <div className="flex justify-between"><span>VELOCITY Y:</span> <span>{velocity.y.toFixed(2)} m/s</span></div>
          </div>
        </div>
      </div>

      {/* Large visual area - Mocking the MATLAB Plot */}
      <div className="glass-panel hud-wrapper" style={{ flex: 1, marginTop: '20px', display: 'flex', flexDirection: 'column' }}>
        <div className="hud-corner hud-tl"></div>
        <div className="hud-corner hud-tr"></div>
        <div className="hud-corner hud-bl"></div>
        <div className="hud-corner hud-br"></div>
        <h3 className="text-xl mb-4 text-green">TRAJECTORY SIMULATION</h3>
        
        <div style={{ flex: 1, background: 'rgba(0,0,0,0.4)', borderRadius: '8px', position: 'relative', overflow: 'hidden', display: 'flex', alignItems: 'center', justifyContent: 'center' }}>
          {/* Mock Trajectory Graph */}
          <svg width="100%" height="100%" style={{ position: 'absolute', opacity: 0.3 }}>
            <defs>
              <pattern id="grid" width="40" height="40" patternUnits="userSpaceOnUse">
                <path d="M 40 0 L 0 0 0 40" fill="none" stroke="var(--color-accent-green)" strokeWidth="0.5"/>
              </pattern>
            </defs>
            <rect width="100%" height="100%" fill="url(#grid)" />
            <path d="M 50 300 Q 150 100 300 250 T 600 150 T 900 200" fill="none" stroke="var(--color-accent-green)" strokeWidth="3" strokeDasharray="10 5" />
          </svg>
          <div className="text-mono text-green font-bold text-2xl" style={{ backgroundColor: 'rgba(15, 23, 42, 0.8)', padding: '10px 20px', borderRadius: '8px', zIndex: 10 }}>
            SIMULATION FEED ONLINE
          </div>
        </div>
      </div>
    </div>
  );
};

export default OperationStrike;
