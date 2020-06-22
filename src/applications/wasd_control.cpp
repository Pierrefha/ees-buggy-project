//
// Created by leonhard on 22.06.20.
//

#include <applications/wasd_control.h>
#include <curses.h>
#include <iostream>

bool wasd_control::init() {
    initscr();
    int row, col;
    getmaxyx(stdscr,row,col);		/* get the number of rows and columns of window*/
    if(row < 3 || col < 20){
        std::cout << "Window to small" << std::endl;
        return false;
    }
    //Return ERR if the user didnt type something in after 1 millisec
    halfdelay(1);
    noecho();
    return true;
}

void wasd_control::print_info(motor_engine *engine, ultrasonic_sensor *dist_sensor, magnetic_sensor *compass) {
    auto rot = compass->get_rotation();
    auto dir = compass->get_direction().normalize();
    auto obst_dist = dist_sensor->calc_distance();
    auto[left_speed, right_speed]  = engine->get_speed_perc();

    mvprintw(0, 0, "Speed left: %.2f   |   Speed right: %.2f", left_speed, right_speed);
    if(!obst_dist){
        mvprintw(1, 0, "No obstacle in front");
    }else{
        mvprintw(1, 0, "Next obstacle in: %.2f", obst_dist->get());
    }
    mvprintw(2, 0, "Rotation: %.2f   |   Direction (%.2f , %.2f)", rot.value, dir.x, dir.y);

}
void wasd_control::run(motor_engine *engine, ultrasonic_sensor *dist_sensor, magnetic_sensor *compass) {
    if(!init()){
        release_resources();
        return;
    }
    /*
     * Remote control of the motor engine using wasd keys.
     */
    char user_cmd = 0;
    while((user_cmd = getch()) != 'x'){
//        clear(); //clear screen
        print_info(engine, dist_sensor, compass);
        refresh();
        /*
         * Check if remote control is stopped. Which is the case if our buggy is
         * not below our safety distance threshold.
         */
        switch (user_cmd){
            case 'e':{
                engine->emergency_stop();
                break;
            }
            case 'q':{
                engine->smooth_stop();
                break;
            }
            case 'w':{
                switch(engine->get_direction()){
                    case direction::IN_PLACE_TURN_RIGHT:
                    case direction::IN_PLACE_TURN_LEFT:
                        engine->smooth_stop();
                        engine->forward();
                        engine->increase_speed();
                        break;
                    case direction::BACKWARDS:
                        engine->decrease_speed();
                        break;
                    case direction::STOP:
                        engine->forward();
                        engine->increase_speed();
                        break;
                    case direction::FORWARD:
                        engine->increase_speed();
                        break;
                }
                break;
            }
            case 's':{
                switch(engine->get_direction()){
                    case direction::IN_PLACE_TURN_RIGHT:
                    case direction::IN_PLACE_TURN_LEFT:
                        engine->smooth_stop();
                        engine->backwards();
                        engine->increase_speed();
                        break;
                    case direction::FORWARD:
                        engine->decrease_speed();
                        break;
                    case direction::STOP:
                        engine->backwards();
                        engine->increase_speed();
                        break;
                    case direction::BACKWARDS:
                        engine->increase_speed();
                        break;
                }
                break;
            }
            case 'd':{
                switch(engine->get_direction()){
                    case direction::IN_PLACE_TURN_RIGHT:
                        engine->increase_speed();
                        break;
                    case direction::IN_PLACE_TURN_LEFT:
                        engine->decrease_speed();
                        break;
                    case direction::FORWARD:
                    case direction::BACKWARDS:
                        engine->turn_right();
                        break;
                    case direction::STOP:
                        engine->turn_in_place_right();
                        engine->increase_speed();
                        break;
                }
                break;
            }
            case 'a':{
                switch(engine->get_direction()){
                    case direction::IN_PLACE_TURN_RIGHT:
                        engine->decrease_speed();
                        break;
                    case direction::IN_PLACE_TURN_LEFT:
                        engine->increase_speed();
                        break;
                    case direction::FORWARD:
                    case direction::BACKWARDS:
                        engine->turn_left();
                        break;
                    case direction::STOP:
                        engine->turn_in_place_left();
                        engine->increase_speed();
                        break;
                }
                break;
            }
            default: break;
        }
    }
    release_resources();
}

void wasd_control::release_resources() {
    endwin();

}



