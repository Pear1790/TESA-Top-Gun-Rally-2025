clc
clear all


m = 4.34; 
g = 9.81;
I = eye(3,3);
I(1,1) = 0.0820;
I(2,2) = 0.0845;
I(3,3) = 0.1377;
L = 0.315; %length of arms


u = [0;0;0;0;0;0];

out = sim('pre_camp.slx');

z = out.yout{1}.Values.Data(3,:,:);
z = squeeze(z);
time = out.yout{1}.Values.Time;

plot(time,z)