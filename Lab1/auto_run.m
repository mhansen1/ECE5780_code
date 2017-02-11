% Clear variables and console.
clear, clc
% Go to execution directory
cd C:/Users/Max/Documents/School/Classes/ECE5780/Lab1

% Got K and J by estimation. From a previous motor that was characterized.
% These values are probably way off, but meh. It's more fun than just
% having values of 1.
J = 0.00085;
K = 0.05;
R = 2.4; % From data sheet. Stall current is 2.5A at 6V (6/2.5=2.4 Ohms)
F = 0.0036; % Motor has a stall torque of 5 oz-in at 12V. Change that to 
            % kg-m and you get his value. This is with no gear box.
F_delay = 0;

ref = 20;
Kp = 0;
Ki = 0;

% Make is so that a figure can be opened.
h = 1;

%%%%%%%%%%%%%%%%%%%
% No friction model
cd no_friction
Ki = 0;
model = 'no_friction';
load_system(model)
figure(h);
hold on
title({'Plot of Motor Model','No Friction or Ki'});
xlabel('Time (s)');
ylabel('Velocity (rad/sec)');
% Find best value for Kp.
try
    for Kp=7:12
        sim(model,2)
        omega = omega_struct.signals.values;
        t = omega_struct.time;
        plot(t,omega)
        hold on      
    end
    plot(t, ones(size(tout))*ref, 'k--');
    legend('Kp = 7','Kp = 8','Kp = 9', 'Kp = 10', 'Kp = 11','Kp = 12','Ref','Location','SouthEast');
    saveas(h, 'no_friction_Kp', 'png');
    close(h);
catch ME
    fprintf('Error! Message: %s\n', ME.message);
    cd ..
end

figure(h);
hold on
title({'Plot of Motor Model','No Friction, Kp = 7'});
xlabel('Time (s)');
ylabel('Velocity (rad/sec)');
% Find the best value for Ki using the best value for Kp.
Ki = 1;
Kp = 7;
try
   for Ki=1:6
        sim(model,2)
        omega = omega_struct.signals.values;
        t = omega_struct.time;
        plot(t,omega)
        hold on
    end
    plot(t, ones(size(tout))*ref, 'k--');
    legend('Ki = 1','Ki = 2','Ki = 3', 'Ki = 4', 'Ki = 5','Ki = 6','Ref','Location','SouthEast');
    saveas(h, 'no_friction_KpKi', 'png');
    close(h);
    cd ..
catch ME
    fprintf('Error! Message: %s\n', ME.message);
    cd ..
end

%%%%%%%%%%%%%%%%%%
% Friction model (continuous)
cd subsystem
Ki = 0;
model = 'subsystem';
load_system(model)
figure(h);
hold on
title({'Plot of Motor Model','With Friction (Continuous), No Ki'});
xlabel('Time (s)');
ylabel('Velocity (rad/sec)');
% Find best value for Kp.
try
    for Kp=7:12
        sim(model,4)
        omega = omega_struct.signals.values;
        t = omega_struct.time;
        plot(t,omega)
        hold on      
    end
    plot(t, ones(size(t))*ref, 'k--');
    legend('Kp = 7','Kp = 8','Kp = 9', 'Kp = 10', 'Kp = 11','Kp = 12','Ref','Location','SouthEast');
    saveas(h, 'continuous_Kp', 'png');
    close(h);
catch ME
    fprintf('Error! Message: %s\n', ME.message);
    cd ..
end

figure(h);
hold on
title({'Plot of Motor Model','With Friction (Continuous), Kp = 3'});
xlabel('Time (s)');
ylabel('Velocity (rad/sec)');
% Find the best value for Ki using the best value for Kp.
Kp = 3;
try
    for Ki=1:6
        sim(model,4)
        omega = omega_struct.signals.values;
        t = omega_struct.time;
        plot(t,omega)
        hold on
    end
    plot(t, ones(size(t))*ref, 'k--');
    legend('Ki = 1','Ki = 2','Ki = 3', 'Ki = 4', 'Ki = 5','Ki = 6','Ref','Location','SouthEast');
    saveas(h, 'continuous_KpKi', 'png');
    close(h);
catch ME
    fprintf('Error! Message: %s\n', ME.message);
    cd ..
end

figure(h)
hold on
title({'Compare P vs. PI Controllers','With Friction (Continuous)'});
xlabel('Time (s)');
ylabel('Velocity (rad/sec)');
Ki = 0;
Kp = 12;
sim(model,4)
omega = omega_struct.signals.values;
t = omega_struct.time;
plot(t,omega,'r')
Ki = 3.2796;
Kp = 0.17*Ki;
sim(model,4)
omega = omega_struct.signals.values;
t = omega_struct.time;
plot(t,omega,'b')
plot(t, ones(size(t))*ref, 'k--');
legend('Kp = 12, Ki = 0','Kp = 0.557532, Ki = 3.2796','Ref','Location','SouthEast');
saveas(h, 'continuous_compare','png');
close(h);
cd ..


%%%%%%%%%%%%%%%%%%%
% Friction model (discrete)
%
cd discrete
%ref = 5;
Ki = 0;
model = 'subsystem_discrete';
load_system(model)
figure(h);
hold on
title({'Plot of Motor Model','With Friction (Discrete), No Ki'});
xlabel('Time (s)');
ylabel('Velocity (rad/sec)');
% Find best value for Kp.
try
    for i=10:15
        Kp = i*0.01;
        sim(model,20)
        omega = omega_struct.signals.values;
        t = omega_struct.time;
        plot(t,omega)
        hold on      
    end
    plot(t, ones(size(t))*ref, 'k--');
    legend('Kp = 0.1','Kp = 0.11','Kp = 0.12', 'Kp = 0.13', 'Kp = 0.14','Kp = 0.15','Ref','Location','SouthEast');
    saveas(h, 'discrete_Kp', 'png');
    close(h);
catch ME
    fprintf('Error! Message: %s\n', ME.message);
    cd ..
end

figure(h);
hold on
title({'Plot of Motor Model','With Friction (Discrete), Kp = 0.5'});
xlabel('Time (s)');
ylabel('Velocity (rad/sec)');
% Find the best value for Ki using the best value for Kp.
Kp = 0.05;
try
    for i=3:8
        Ki = i*0.01;
        sim(model,20)
        omega = omega_struct.signals.values;
        t = omega_struct.time;
        plot(t,omega)
        hold on
    end
    plot(t, ones(size(t))*ref, 'k--');
    legend('Ki = 0.03','Ki = 0.04','Ki = 0.05', 'Ki = 0.06', 'Ki = 0.07','Ki = 0.08','Ref','Location','SouthEast');
    saveas(h, 'Discrete_KpKi', 'png');
    close(h);
catch ME
    fprintf('Error! Message: %s\n', ME.message);
    cd ..
end

figure(h)
hold on
title({'Compare P vs. PI Controllers','With Friction (Discrete)'});
xlabel('Time (s)');
ylabel('Velocity (rad/sec)');
Ki = 0;
Kp = 0.05;
sim(model,20)
omega = omega_struct.signals.values;
t = omega_struct.time;
plot(t,omega,'r')
Ki = 0.06;
Kp = 0.01;
sim(model,20)
omega = omega_struct.signals.values;
t = omega_struct.time;
plot(t,omega,'b')
plot(t, ones(size(t))*ref, 'k--');
legend('Kp = 0.05, Ki = 0','Kp = 0.01, Ki = 0.06','Ref','Location','SouthEast');
saveas(h, 'Discrete_compare','png');
close(h);
cd ..



