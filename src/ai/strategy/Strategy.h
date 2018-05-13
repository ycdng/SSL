#ifndef __STRATEGY__STRATEGY__H__
#define __STRATEGY__STRATEGY__H__

#include <robot_behavior/robot_behavior.h>
#include <map>
#include <memory>
#include <AiData.h>

namespace RhobanSSL {
namespace Strategy {

class Strategy {
    protected:
    Ai::AiData & ai_data;
    private:
    int goalie_id;
    int goalie_opponent_id;
    std::vector<int> robot_ids;
    std::vector<int> player_ids;

    void update_player_ids();

    public:

    Strategy(Ai::AiData & ai_data);

    void set_goalie( int id );
    void set_goalie_opponent( int id );
    
    // Get the goalie id. If id<0 then no goalie is declared 
    int get_goalie() const;   
    // Get the opponent goalie id. If id<0 then no opponent goalie is declared  
    int get_goalie_opponent() const;   

    void set_robot_affectation( const std::vector<int> & robot_ids );
    
    const std::vector<int> & get_robot_ids() const;
    const std::vector<int> & get_player_ids() const;

    int robot_id( int id ) const;
    int player_id( int id ) const;


    virtual void update(double time){};

    virtual void start(double time){};
    virtual void stop(double time){};
    virtual void pause(double time){};
    virtual void resume(double time){};

    /*
     * This function give the minimal numer of robot 
     * that strategy command.
     */
    virtual int min_robots() const = 0;
    /*
     * This function give the maximal numer of robot 
     * that strategy command.
     * if it is set to -1, then the strategy can command any number of robot.
     */
    virtual int max_robots() const = 0;


    virtual void assign_behavior_to_robots(
        std::function<
            void (int, std::shared_ptr<Robot_behavior::RobotBehavior>)
        > assign_behavior,
        double time, double dt
    ) = 0;

    virtual ~Strategy();
};

};
};
#endif