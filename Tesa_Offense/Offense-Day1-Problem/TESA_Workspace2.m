%ทำไฟล์นี้

clc
clear all

waypoints = [ 0 0 0 0; %x y z yo
    0 0 -10 1;
    10 10 -10 1;
    10 30 -20 1;
    30 40 -30 1];
timepoints = [0 5 10 15 20];%กำหนดวินาที

SimulationTime = 20;
timestep = 0.01;

trajectory_coef = min_snap_Coef(waypoints,timepoints);


X =[];
Y = [];
Z = [];
Psi = [];
dX = [];
dY = [];
dZ = [];
dPsi = [];
ddX = [];
ddY = [];
ddZ = [];
ddPsi = [];
dddX = [];
dddY = [];
dddZ = [];
ddddX = [];
ddddY = [];
ddddZ = [];


%เก็บค่าของไดเร็กทรอรีในค่าดิสแคป
tX = 0;
tY = 0;
tZ = 0;
tPsi = 0;
tdX = 0;
tdY = 0;
tdZ = 0;
tdPsi = 0;
tddX = 0;
tddY = 0;
tddZ = 0;
tddPsi = 0;
for t = 0:timestep:SimulationTime
[tX,tY,tZ,tPsi,tdX,tdY,tdZ,tdPsi,tddX,tddY,tddZ,tddPsi,tdddX,tdddY,tdddZ,tddddX,tddddY,tddddZ] = getTrajectory(t,trajectory_coef,timepoints);
X = [X tX];
Y = [Y tY];
Z = [Z tZ];
Psi = [Psi tPsi];
dX = [dX tdX]; %ดิฟ1 ครั้ง
dY = [dY tdY];
dZ = [dZ tdZ];
dPsi = [dPsi tdPsi];
ddX = [ddX tddX]; % ดิฟ 2 ครั้ง
ddY = [ddY tddY];
ddZ = [ddZ tddZ];
ddPsi = [ddPsi tddPsi];
dddX = [dddX tdddX]; % ดิฟ 3 ครััง
dddY = [dddY tdddY];
dddZ = [dddZ tdddZ];
ddddX = [ddddX tddddX]; % ดริฟ 4 ครั้ง
ddddY = [ddddY tddddY];
ddddZ = [ddddZ tddddZ];

end
% f1 = figure;
% plot3(X,Y,Z);
% f1.CurrentAxes.ZDir = 'Reverse';
% xlabel('x')
% ylabel('y')
% zlabel('-z')

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%%% Create Variable in Simulick Workspace %%%%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%


% properties of drones
m = 4.34;  %มวล
g = 9.81; %ค่าแรงโนมถ่วง
dIdt = zeros(3,3);
I = eye(3,3); % initial matrix
I(1,1) = 0.0820; %identity matrix 
I(2,2) = 0.0845;
I(3,3) = 0.1377;
L = 0.315; %length of arms


cf = 8.004e-4; % ไว้คำนวนท็อก

% K matrix to convert motor speed to force and torques u = K . omega2
K = [1 1 1 1;
    0 -L 0 L;
    L 0 -L 0;
    -cf cf -cf cf];

%ตั้งแต่ตอนนี้เป็นค่าเกณฑ์
Kp = eye(3,3);
Kp = 1*Kp; 
Kv = eye(3,3);
Kv = 1*Kv; 
Kr = eye(3,3);
Kr = 1*Kr;
Kw = eye(3,3);
Kw = 1*Kw; 

% เอา ไดเร็กทอรี่ไปไว้ในซิมมูลิงค์
time = 0:timestep:SimulationTime;
inputStructure.time = time;
inputStructure.signals(1).values = X';
inputStructure.signals(1).dimensions = 1;
inputStructure.signals(2).values = Y';
inputStructure.signals(2).dimensions = 1;
inputStructure.signals(3).values = Z';
inputStructure.signals(3).dimensions = 1;

inputStructure.signals(4).values = dX';
inputStructure.signals(4).dimensions = 1;
inputStructure.signals(5).values = dY';
inputStructure.signals(5).dimensions = 1;
inputStructure.signals(6).values = dZ';
inputStructure.signals(6).dimensions = 1;

inputStructure.signals(7).values = ddX';
inputStructure.signals(7).dimensions = 1;
inputStructure.signals(8).values = ddY';
inputStructure.signals(8).dimensions = 1;
inputStructure.signals(9).values = ddZ';
inputStructure.signals(9).dimensions = 1;

inputStructure.signals(10).values = dddX';
inputStructure.signals(10).dimensions = 1;
inputStructure.signals(11).values = dddY';
inputStructure.signals(11).dimensions = 1;
inputStructure.signals(12).values = dddZ';
inputStructure.signals(12).dimensions = 1;

inputStructure.signals(13).values = ddddX';
inputStructure.signals(13).dimensions = 1;
inputStructure.signals(14).values = ddddY';
inputStructure.signals(14).dimensions = 1;
inputStructure.signals(15).values = ddddZ';
inputStructure.signals(15).dimensions = 1;


inputStructure.signals(16).values = Psi';
inputStructure.signals(16).dimensions = 1;
inputStructure.signals(17).values = dPsi';
inputStructure.signals(17).dimensions = 1;
inputStructure.signals(18).values = ddPsi';
inputStructure.signals(18).dimensions = 1;


%ทำการ ซิมมูเลสค่าไฟล์
out = sim('TESAOffense50Day2.slx');

% เอาค่ามาพ็อตไว้
Xe = out.yout{1}.Values.Data(:,1)';
Ye = out.yout{1}.Values.Data(:,2)';
Ze = out.yout{1}.Values.Data(:,3)';
f2 = figure;
plot3(X, Y, Z, '-b', 'LineWidth', 2); % ฟ้า
hold on
plot3(Xe, Ye, Ze, 'r', 'LineWidth', 2); % ส้ม
f2.CurrentAxes.ZDir = 'Reverse';
xlabel('x'); ylabel('y'); zlabel('-z');
legend('Desired Trajectory', 'Actual Trajectory', 'Location', 'best');
grid on


% rotor1 = out.yout{2}.Values.Data;
% rotor2 = out.yout{3}.Values.Data;
% rotor3 = out.yout{4}.Values.Data;
% rotor4 = out.yout{5}.Values.Data;
% 
% figure
% plot(time,rotor1);
% figure
% plot(time,rotor2);
% figure
% plot(time,rotor3);
% figure
% plot(time,rotor4);