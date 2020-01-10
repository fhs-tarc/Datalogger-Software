#include <Arduino.h>

struct Quaternion {double w, x, y, z;};
struct EulerAngles {double roll, pitch, yaw;};

Quaternion toQuaternion(double yaw, double pitch, double roll) {
    double cy = cos(yaw * 0.5);
    double sy = sin(yaw * 0.5);
    double cp = cos(pitch * 0.5);
    double sp = sin(pitch * 0.5);
    double cr = cos(roll * 0.5);
    double sr = sin(roll * 0.5);

    Quaternion q;
    q.w = cy * cp * cr + sy * sp * sr;
    q.x = cy * cp * sr - sy * sp * cr;
    q.y = sy * cp * sr + cy * sp * cr;
    q.z = sy * cp * cr - cy * sp * sr;

    return q;
}

EulerAngles toEuler(Quaternion q) {
    EulerAngles angles;

    // Roll
    double sinr_cosp = 2 * (q.w*q.x + q.y*q.z);
    double cosr_cosp = 1 - 2 * (q.x*q.x + q.y*q.y);

    angles.roll = atan2(sinr_cosp, cosr_cosp);

    // pitch
    double sinp = 2 * (q.w*q.y - q.z*q.x);
    if(abs(sinp) >= 1)  angles.pitch = copysign(M_PI/2, sinp);
    else angles.pitch = asin(sinp);

    // yaw
    double siny_cosp = 2 * (q.w*q.z + q.x*q.y);
    double cosy_cosp = 1 - 2 * (q.y*q.y + q.z*q.z);
    angles.yaw = atan2(siny_cosp, cosy_cosp);

    return angles;
}

Quaternion quatMultiply(Quaternion q1, Quaternion q2) {
    Quaternion q;

    q.w = q1.w*q2.w - q1.x*q2.x - q1.y*q2.y - q1.z*q2.z;
    q.x = q1.w*q2.x + q1.x*q2.w + q1.y*q2.z - q1.z*q2.y;
    q.y = q1.w*q2.y - q1.x*q2.z + q1.y*q2.w + q1.z*q2.x;
    q.z = q1.w*q2.z + q1.x*q2.y - q1.y*q2.x + q1.z*q2.w;

    return q;
}

double norm(double x, double y) {
    return sqrt(x*x + y*y);
}

double norm(double x, double y, double z) {
    return sqrt(x*x + y*y + z*z);
}

double norm(double w, double x, double y, double z) {
    return sqrt(w*w + x*x + y*y + z*z);
}

double norm(Quaternion q) {
    return sqrt(q.w*q.w + q.x*q.x + q.y*q.y + q.z*q.z);
}

Quaternion deltaQ(double t, double x, double y, double z) {
    double n = norm(x, y, z);
    Quaternion q;

    q.w = cos(t * n/2);
    q.x = (x/n) * sin(t * n/2);
    q.y = (y/n) * sin(t * n/2);
    q.z = (z/n) * sin(t * n/2);

    n = norm(q);

    q.w /= n;
    q.x /= n;
    q.y /= n;
    q.z /= n;

    return q;
}
