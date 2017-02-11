y = -pi/4;
x = 0;
z = pi;
g = 9.81;
dt = 1e-3;

R_BI = [cos(z)*cos(y) cos(z)*sin(x)*sin(y)-cos(x)*sin(z) sin(x)*sin(z)+cos(x)*cos(z)*sin(y); ...
        cos(y)*sin(z) cos(x)*cos(z)+sin(x)*sin(z)*sin(y) cos(x)*sin(z)*sin(y)-cos(z)*sin(x); ...
        -sin(y) cos(y)*sin(x) cos(x)*cos(y)];
 
R_IB = [cos(z)*cos(y) cos(y)*sin(z) -sin(y); ...
        cos(z)*sin(x)*sin(y)-cos(x)*sin(z) cos(x)*cos(z)+sin(x)*sin(z)*sin(y) cos(y)*sin(x); ...
        sin(x)*sin(z)+cos(x)*cos(z)*sin(y) cos(x)*sin(z)*sin(y)-cos(z)*sin(x) cos(x)*cos(y)];
 

v_b1 = [cos(pi/2-abs(y))*g; ...
        0; ...
        cos(pi/2-abs(y))*g];
v_i1 = [0; ...
        0; ...
        g];
    
v_i2 = R_BI*v_b1
    

     
     
     
     
     
     
     
     
     
     
     
     
     
