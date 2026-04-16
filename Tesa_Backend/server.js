const express = require('express');
const http = require('http');
const { Server } = require('socket.io');
const cors = require('cors');

const app = express();
app.use(cors());
app.use(express.json());

const server = http.createServer(app);
const io = new Server(server, {
  cors: {
    origin: "*", // allow front-end to connect
    methods: ["GET", "POST"]
  }
});

io.on('connection', (socket) => {
  console.log('Client connected:', socket.id);
  socket.on('disconnect', () => {
    console.log('Client disconnected:', socket.id);
  });
});

// Endpoint for Tesa_Offense (MATLAB)
app.post('/api/offense_telemetry', (req, res) => {
  const data = req.body;
  // Broadcast to all connected Dashboard clients
  io.emit('offense_data', data);
  res.status(200).send('Offense data received');
});

// Endpoint for Tesa_Defense (YOLO/Python)
app.post('/api/defense_telemetry', (req, res) => {
  const data = req.body;
  // Broadcast to all connected Dashboard clients
  io.emit('defense_data', data);
  res.status(200).send('Defense data received');
});

const PORT = 3000;
server.listen(PORT, () => {
  console.log(`🚀 TESA Backend Command Center running on port ${PORT}`);
});
