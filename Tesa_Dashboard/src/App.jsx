import React from 'react';
import { BrowserRouter as Router, Routes, Route, Navigate } from 'react-router-dom';
import Sidebar from './components/Sidebar';
import OperationStrike from './pages/OperationStrike';
import SentinelWatch from './pages/SentinelWatch';

function App() {
  return (
    <Router>
      <div className="app-container">
        <Sidebar />
        <main className="main-content">
          <Routes>
            <Route path="/" element={<Navigate to="/strike" replace />} />
            <Route path="/strike" element={<OperationStrike />} />
            <Route path="/sentinel" element={<SentinelWatch />} />
          </Routes>
        </main>
      </div>
    </Router>
  );
}

export default App;
