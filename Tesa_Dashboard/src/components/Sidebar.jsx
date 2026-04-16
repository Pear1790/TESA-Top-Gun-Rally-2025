import React from 'react';
import { NavLink, useLocation } from 'react-router-dom';
import { Target, ShieldAlert, Zap } from 'lucide-react';

const Sidebar = () => {
  const location = useLocation();
  const isStrike = location.pathname.includes('strike');
  
  return (
    <div className={`sidebar`} style={{ borderRightColor: isStrike ? 'var(--color-accent-green)' : 'var(--color-accent-red)' }}>
      <div className="flex items-center gap-2 mb-6" style={{ padding: '0 10px' }}>
        <Zap className={isStrike ? 'text-green' : 'text-red'} size={28} />
        <div>
          <h1 className="text-xl font-bold">TESA</h1>
          <p className="text-secondary" style={{ fontSize: '0.8rem', letterSpacing: '2px' }}>COMMAND CENTER</p>
        </div>
      </div>

      <div style={{ flex: 1, marginTop: '20px' }}>
        <p className="text-secondary mb-2" style={{ fontSize: '0.75rem', textTransform: 'uppercase', paddingLeft: '16px' }}>Offense Mode</p>
        <NavLink 
          to="/strike" 
          className={({ isActive }) => `nav-link ${isActive ? 'active-strike' : ''}`}
        >
          <Target size={20} />
          <span>Operation Strike</span>
        </NavLink>

        <p className="text-secondary mb-2 mt-6" style={{ fontSize: '0.75rem', textTransform: 'uppercase', paddingLeft: '16px' }}>Defense Mode</p>
        <NavLink 
          to="/sentinel" 
          className={({ isActive }) => `nav-link ${isActive ? 'active-sentinel' : ''}`}
        >
          <ShieldAlert size={20} />
          <span>Sentinel Watch</span>
        </NavLink>
      </div>

      <div className="glass-panel" style={{ padding: '1rem', marginTop: 'auto', background: 'rgba(0,0,0,0.3)' }}>
        <div className="flex items-center gap-2 mb-2">
          <span className="status-indicator status-active"></span>
          <span className="text-mono" style={{ fontSize: '0.85rem' }}>SYSTEM ONLINE</span>
        </div>
        <div className="text-secondary text-mono" style={{ fontSize: '0.75rem' }}>
          LAT: 13.7367 N <br/>
          LON: 100.5231 E
        </div>
      </div>
    </div>
  );
};

export default Sidebar;
