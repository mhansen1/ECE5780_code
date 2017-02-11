Max:

IMU:
Pulls in the data over I2C, then converts it from the gyro's form into a floating point number.
During the initialization, the gyro and accelerometer offsets are calculated. This is done by reading
the values in a certain number of time (right now it's set to 2,000), then averages the values. This is
still not 100% accurate, and something needs to be done about recalibration after a certain amount 
of time. However, the error is decreasing and right now is at about plus/minus 0.025 m/s.s error for
the accelerometer. 
The actual update is run on a timer. The period of this timer is set during the initialization. When 
the timer fires, the data is pulled in, and converted to a floating point format. Then to remove
gravity, and Euler angle transformation matrix is used. Ideally this would remove all of the 
force due to gravity, but due to the offsets still being off and the varience in gravity from location
to location, this still isn't perfect.
As for filters, ideally, a Kalman filter would be used. However, because this can be complicated 
for a beginner, a simple running average filter is being run on the accelerometer data. The gyro
doesn't seem to need filtering since it's data is reliable, other than drift. Also, to help compensate
for drift, a complementary filter is used. This uses a portion of the of the accelerometer data 
(angle found using the atan2() function) along with the gyro data.

Kalman:
Ideally, kinetic data (linear and rotational) would be filtered using a Kalman filter to give accurate
data despite noise. Right now, it's not all that great. In order to make it better, the filter is being
tested in Matlab, since it makes using matrices easy. The code is a bit out of date right now
compared to the written notes, so it needs to be updated and tested.