//
// Created by leonhard on 22.06.20.
//

#include <applications/wasd_control.h>
#include <curses.h>
#include <iostream>
#include <driving/automatic_movement.h>

bool wasd_control::init() {
    initscr();
    int row, col;
    getmaxyx(stdscr,row,col);		/* get the number of rows and columns of window*/
    if(row < 5 || col < 20){
        std::cout << "Window to small" << std::endl;
        return false;
    }
    cbreak();
    nodelay(stdscr,true);
    noecho();
    clear();
    return true;
}

void wasd_control::print_info(std::tuple<float, float> speed_perc,
                              std::optional<cm> obst_dist,
                              degree<float> rot,
                              vertex2D<float> dir) {
    auto[left_speed, right_speed]  = speed_perc;

    mvprintw(0, 0, "Speed left: %.2f   |   Speed right: %.2f", left_speed, right_speed);
    if(!obst_dist){
        mvprintw(1, 0, "No obstacle in front");
        //Clear next obstacle dist
        clrtoeol();
    }else{
        mvprintw(1, 0, "Next obstacle in: %.2f", obst_dist->get());
    }
    mvprintw(2, 0, "Rotation: %.2f   |   Direction (%.2f , %.2f)", rot.value, dir.x, dir.y);

}
void wasd_control::run(motor_engine *engine, ultrasonic_sensor *dist_sensor, compass *compass) {
    if(!init()){
        release_resources();
        return;
    }
	bool backlight_on = false;
    /*
     * Remote control of the motor engine using wasd keys.
     */
    while(true){
        //Remove all old user inputs. New user inputs will accumulate till getch()
        flushinp();
        auto obst_dist = dist_sensor->calc_distance();
        print_info(engine->get_speed_perc(), obst_dist, compass->get_rotation(), compass->get_direction());

        bool forward_movement_possible = check_forward_movement_possible(obst_dist);
        if(!forward_movement_possible){
		if(engine->get_direction() == movement_type::FORWARD){
            engine->smooth_stop();
            dist_sensor->set_brake_light(ON);
	    backlight_on = true;
		}
        }else{
	}

        int user_cmd = getch();
        if(user_cmd == 'x'){
            break;
        }
	if(backlight_on && (user_cmd == 's' || user_cmd == 'a' || user_cmd == 'd')){

            dist_sensor->set_brake_light(OFF);
	}
        print_user_input(user_cmd);

        //Commit changes to screen
        refresh();

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
                if(!forward_movement_possible){
                    continue;
                }
                switch(engine->get_direction()){
                    case movement_type::IN_PLACE_TURN_RIGHT:
                    case movement_type::IN_PLACE_TURN_LEFT:
                        engine->smooth_stop();
                        engine->forward();
                        engine->increase_speed();
                        break;
                    case movement_type::BACKWARDS:
                        engine->decrease_speed();
                        break;
                    case movement_type::STOP:
                        engine->forward();
                        engine->increase_speed();
                        break;
                    case movement_type::FORWARD:
                        engine->increase_speed();
                        break;
                }
                break;
            }
            case 's':{
                switch(engine->get_direction()){
                    case movement_type::IN_PLACE_TURN_RIGHT:
                    case movement_type::IN_PLACE_TURN_LEFT:
                        engine->smooth_stop();
                        engine->backwards();
                        engine->increase_speed();
                        break;
                    case movement_type::FORWARD:
                        engine->decrease_speed();
                        break;
                    case movement_type::STOP:
                        engine->backwards();
                        engine->increase_speed();
                        break;
                    case movement_type::BACKWARDS:
                        engine->increase_speed();
                        break;
                }
                break;
            }
            case 'd':{
                switch(engine->get_direction()){
                    case movement_type::IN_PLACE_TURN_RIGHT:
                        engine->increase_speed();
                        break;
                    case movement_type::IN_PLACE_TURN_LEFT:
                        engine->decrease_speed();
                        break;
                    case movement_type::FORWARD:
                    case movement_type::BACKWARDS:
                        engine->turn_right();
                        break;
                    case movement_type::STOP:
                        engine->turn_in_place_right();
                        engine->increase_speed();
                        break;
                }
                break;
            }
            case 'a':{
                switch(engine->get_direction()){
                    case movement_type::IN_PLACE_TURN_RIGHT:
                        engine->decrease_speed();
                        break;
                    case movement_type::IN_PLACE_TURN_LEFT:
                        engine->increase_speed();
                        break;
                    case movement_type::FORWARD:
                    case movement_type::BACKWARDS:
                        engine->turn_left();
                        break;
                    case movement_type::STOP:
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


bool wasd_control::check_forward_movement_possible(std::optional<cm> obst_dist) {
    clear_obstacle_warnings();
    if(!obst_dist){
        return true;
    }
    if(obst_dist->get() < 20){
        print_no_forward_movement();
        return false;
    }
    if(obst_dist->get() < 40){
        print_warning();
        return true;
    }
    return true;
}

void wasd_control::clear_obstacle_warnings() {
    move(4,0);
    clrtoeol();
}

void wasd_control::print_warning() {
    mvprintw(4, 0, "Obstacle in front! Slow down!");
}

void wasd_control::print_no_forward_movement() {
    mvprintw(4,0, "Obstacle in front! Forward movement disabled");
}

void wasd_control::print_user_input(int cmd) {
    if(cmd == ERR){
        mvprintw(5,0,"No user input");
    }else{
        mvprintw(5,0,"Your input %c", cmd);
        //Clear the t of input if there was cmd == ERR before
        clrtoeol();
    }
}


