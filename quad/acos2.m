function [ angle ] = acos2( a,h )
%ACOS2 Summary of this function goes here
%   Detailed explanation goes here

angle = acos(a/h);
if a<0
    angle = -angle;
end

end

