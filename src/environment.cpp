#include "../include/environment.hpp"

double gravity_value = 0.1;

Environment::Environment() : 
    rods({
        Rod({-7.0,0.0,0.0},{1.0,0.0,0.0,1.0f}),
        Rod({0.0,0.0,0.0},{1.0,0.0,0.0,1.0f}),
        Rod({7.0,0.0,0.0},{1.0,0.0,0.0,1.0f})}), 
    disks({
        Disk({-7,-5,0},{0.0f, 1.0f, 0.0f, 1.0f},0.35,2.75),
        Disk({-7,-4,0},{0.0f, 1.0f, 0.0f, 1.0f},0.35,2.5),
        Disk({-7,-3,0},{0.0f, 1.0f, 0.0f, 1.0f},0.35,2.25),
        Disk({-7,-2,0},{0.0f, 1.0f, 0.0f, 1.0f},0.35,2),
        Disk({-7,-1,0},{0.0f, 1.0f, 0.0f, 1.0f},0.35,1.75),
        Disk({-7,0,0},{0.0f, 1.0f, 0.0f, 1.0f},0.35,1.5),
        Disk({-7,1,0},{0.0f, 1.0f, 0.0f, 1.0f},0.35,1.25),
        Disk({-7,2,0},{0.0f, 1.0f, 0.0f, 1.0f},0.35,1)}){
    
    for(auto & i:disks){
        stacks_array[0].push_front(&i);  
    }
    current_selected = false;
    selected_rod = 0;
    rods[0].highlight();
    move_counter = 0;
}

void Environment::draw(){
    for(auto &i: rods){
        i.draw();
    }
    for(auto &j: disks){
        j.draw();
    }
    apply_gravity();
}


static int translateSelection(rod_selected sel) {
    switch (sel) {
        case rod_selected::left:
            return 0;
        case rod_selected::middle:
            return 1;
        case rod_selected::right:
            return 2;

    }
}
bool Environment::select(rod_selected rod){
    if(!current_selected) {
        int index = translateSelection(rod);
        auto &s = stacks_array[index];
        if(s.empty()){
            current_selected = false;
            return false;
        } else{
            current_selected = true;
            selected = s.front();
            last_point = selected->get_origin();
            s.pop_front();
            return true;
        }
    }else {
        return false;
    }
}

void Environment::move(direction dir){
    if(current_selected) {
        switch(dir){
            case direction::up:
                if(can_move_selected(dir)){
                    selected->move(vector({0,0.5,0}));
                }
                break;
            case direction::down:
                if(can_move_selected(dir)){
                    selected->move(vector({0,-0.5,0}));
                }
                break;
            case direction::right:
                if(can_move_selected(dir)){
                    selected->move(vector({0.5,0,0}));
                }
                break;
            case direction::left:
                if(can_move_selected(dir)){
                    selected->move(vector({-0.5,0,0}));
                }
                break;
        }
    }
}


bool Environment::can_move_selected(direction dir){
    switch(dir){
        case direction::up:
            return true;
        case direction::down:
            if(selected->get_origin().y <= -5){
               return false;
            }
            else if(1){

            }
            return true;
        case direction::right:
            if((inRange(selected->get_origin().x, -8,-6)||
                inRange(selected->get_origin().x, -1, 1)||
                inRange(selected->get_origin().x, 6, 8))
                && inRange(selected->get_origin().y,-5 , 5)){
                return false;
            }
            return true;
        case direction::left:
            if((inRange(selected->get_origin().x, -8,-6)||
                inRange(selected->get_origin().x, -1, 1)||
                inRange(selected->get_origin().x, 6, 8)
                && inRange(selected->get_origin().y,-5 , 5))){
                return false;
            }
            return true;
    }
}

void Environment::highLight(rod_selected rod){
    rods[selected_rod].unhighlight();
    switch(rod){
        case rod_selected::left:
            selected_rod = 0;
            break;
        case rod_selected::middle:
            selected_rod = 1;
            break;
        case rod_selected::right:
            selected_rod = 2;
            break;
    }
    rods[selected_rod].highlight();
}
bool Environment::inRange(float v, float min, float max) {
    return (v >= min) && (v <= max);
}

bool Environment::should_move_back(){
    Point current_point = selected->get_origin();

    if(inRange(current_point.x, -8, -6) || inRange(current_point.x, -1, 1)
        || inRange(current_point.x , 6, 8)){

        if(current_point.y >= -5 && current_point.y <=5)
        {

            if(current_point.z == 0){
                if(inRange(current_point.x, -8,-6)){
                    auto &s = stacks_array[0];
                    if(s.empty()) {
                        return false;
                    }else if( !(selected > s.front())){
                        return true;
                    }else {
                        return false;
                    }
                }
                else if(inRange(current_point.x, -1, 1)){
                    auto &s = stacks_array[1];
                    if(s.empty()) {
                        return false;
                    }else if(!(selected > s.front())){
                        return true;
                    }else {
                        return false;
                    }
                }
                else if(inRange(current_point.x, 6, 8)){
                    auto &s = stacks_array[2];
                    if(s.empty()) {
                        return false;
                    }else if(!(selected > s.front())){
                        return true;
                    }else {
                        return false;
                    }
                }
            }

            return true;
        }else {
            return true;
        }
    } else {
        return true;
    }
}

void Environment::unselect(){
    if(current_selected) {
        if(should_move_back()){
            selected->set_origin(last_point);
            stacks_array[selected_rod].push_front(selected);

        }else{
            move_counter++;
            if(inRange(selected->get_origin().x, -8, -6)) {
                stacks_array[0].push_front(selected);
            } else if (inRange(selected->get_origin().x, -1 , 1) ){
                stacks_array[1].push_front(selected);
            }else if (inRange(selected->get_origin().x, 6, 8) ) {
                stacks_array[2].push_front(selected);
            }

        }   
        
    }
    current_selected = false;
}

void Environment::apply_gravity(){
     for(auto &i: disks){
        if(can_move(i)){
            i.move(vector({0.0,-0.1,0.0}));
        }    
    }
}

//collision detection
bool Environment::can_move(Disk &d){ 
    if(d.get_origin().y <= -4.5){
        return false;
    } 
    if(current_selected && (*selected == d)) {
        return false;
    }
    if(inRange(d.get_origin().x, -8, -6)) {
        if(stacks_array[0].empty()) {
            return true;
        }else if(stacks_array[0].size() == 1) {
            return true;
        }
        auto  iter = stacks_array[0].begin();
        while(iter != stacks_array[0].end() && **iter != d) {
            iter++;
        }
        if(iter != stacks_array[0].end()){
            iter++;
            if(iter != stacks_array[0].end()) {
                if(d.get_origin().y > (*iter)->get_origin().y + 1 ) {
                    return true;
                }else {
                    return false;
                }
            }else {
                return false;
            }
        } 
    }else if(inRange(d.get_origin().x, -1, 1)) {
        if(stacks_array[1].empty() ){
            return true;
        }else if(stacks_array[1].size() == 1) {
            return true;
        }
        auto  iter = stacks_array[1].begin();
        while(iter != stacks_array[1].end() && **iter != d) {
            iter++;
        }
        if(iter != stacks_array[1].end()){
            iter++;
            if(iter != stacks_array[1].end()) {
                if(d.get_origin().y > (*iter)->get_origin().y + 1 ) {
                    return true;
                }else {
                    return false;
                }
            }else {
                return false;
            }
        }
    }else if(inRange(d.get_origin().x, 6, 8)) {
        if(stacks_array[2].empty()) {
            return true;
        }else if(stacks_array[2].size() == 1) {
            return true;
        }
        auto  iter = stacks_array[2].begin();
        while(iter != stacks_array[2].end() && **iter != d) {
            iter++;
        }
        if(iter != stacks_array[2].end()){
            iter++;
            if(iter != stacks_array[2].end()) {
                if(d.get_origin().y > (*iter)->get_origin().y + 1 ) {
                    return true;
                }else {
                    return false;
                }
            }else {
                return false;
            }
        }
    }
    return false;
}

bool Environment::gameOver(){
    return stacks_array[2].size() == 8;
}