function [ angle ] = get_angle( a,h )
%GET_ANGLE Summary of this function goes here
%   Detailed explanation goes here

angle = acos2(a,h);

if angle<0
    angle = angle + pi/2;
else
    angle = pi/2 - angle;
end

end

