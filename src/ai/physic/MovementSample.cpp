#include "MovementSample.h"
#include <assert.h>
#include <debug.h>

using namespace rhoban_geometry;

namespace RhobanSSL {

PositionSample::PositionSample():
    PositionSample( 0.0, Point(0.0, 0.0), ContinuousAngle(0.0) )
{ }

PositionSample::PositionSample(
    double time,
    const Point & linear_position,
    const ContinuousAngle & angular_position
):
    time(time), linear_position(linear_position),
    angular_position(angular_position)
{ }


void MovementSample::insert( const PositionSample & sample ){
    assert( sample.time >= (*this)[0].time );
    if( sample.time == (*this)[0].time ){
        (*this)[0] = sample;
    }else{
        circular_vector<PositionSample>::insert( sample );
    }
}

bool MovementSample::is_valid() const {
    for( int i=0; i<this->size()-1; i++ ){
        if( (*this)[i].time <= (*this)[i+1].time ){
            return false;
        }
    }
    return true;
}

MovementSample::MovementSample(unsigned int size):
    circular_vector<PositionSample>(size)
{ }

MovementSample::MovementSample():
    circular_vector<PositionSample>()
{ }

double MovementSample::time( unsigned int i ) const {
    return (*this)[i].time;
}

double MovementSample::dt( unsigned int i ) const {
    return (*this)[i].time - (*this)[i+1].time;
}

Point MovementSample::linear_position( unsigned int i ) const {
    return (*this)[i].linear_position;
}

ContinuousAngle MovementSample::angular_position( unsigned int i ) const{
    return (*this)[i].angular_position;
}

Vector2d MovementSample::linear_velocity( unsigned int i ) const {
    return ( linear_position(i) - linear_position(i+1) )/dt(i);
}

ContinuousAngle MovementSample::angular_velocity( unsigned int i ) const{
    return ( angular_position(i) - angular_position(i+1) )/dt(i);
}

Vector2d MovementSample::linear_acceleration( unsigned int i ) const {
    return ( linear_velocity(i) - linear_velocity(i+1) )/dt(i);
}

ContinuousAngle MovementSample::angular_acceleration( unsigned int i ) const {
    return ( angular_velocity(i) - angular_velocity(i+1) )/dt(i);
}

}

std::ostream& operator<<(
    std::ostream& stream, const RhobanSSL::PositionSample & pos
){
    stream << "("
        "t=" << pos.time << ", "
        "lin=" << pos.linear_position << ", "
        "ang=" << pos.angular_position << ")";
    return stream;
}


std::ostream& operator<<(
    std::ostream& stream, const RhobanSSL::MovementSample & mov
){
    stream << static_cast<circular_vector<RhobanSSL::PositionSample>>(mov);
    return stream;
}
