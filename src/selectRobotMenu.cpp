#include "../inc/selectRobotMenu.hpp"

using namespace Engine;

SelectRobotMenu::SelectRobotMenu(
    std::string screen_name
){
    set_name(screen_name);
}

void SelectRobotMenu::init(){
    init_user();
    init_challenge_creator();
    init_background();
    init_texts();
    init_selector();
    init_robots();
    init_select_challenge_objects();
    init_init_button();
}

void SelectRobotMenu::init_background(){
    background_field = new Field(
        "selector_field",
        background_position,
        std::make_pair(390,258)
    );
    add_object(background_field);
}

void SelectRobotMenu::init_selector(){
    selector_field = new Selector(
        "selector_field",
        selector_field_position,
        std::make_pair(70,70),
        2,
        3,
        100
    );
    selector_field->set_sprite(
        "./assets/sprites/tiles/border.png"
    );
    add_object(selector_field);
}

void SelectRobotMenu::init_robots(){
    Engine::Collider& collider = Engine::Collider::get_instance();
    std::string robot_base_path = "./assets/sprites/bots/bot_";
    std::pair<int, int> aux_robot_position;
    int line_index = 0;
    int column_index = 0;
    for(int i = 97; i < 103; i++){
        char aux_letter = i;
        aux_robot_position.first = (
            robots_position.first + (column_index * 128) + 32
        );
        aux_robot_position.second = (
            robots_position.second + (line_index * 128) + 32
        );

        Field* aux_field = new Field(
            "robot_" + i,
            aux_robot_position,
            std::make_pair(64,64)
        );
        aux_field->set_sprite(
            robot_base_path+aux_letter+".png",
            ""
        );
        robots.push_back(aux_field);
        collider.add_object(aux_field);
        add_object(aux_field);

        column_index++;
        if(column_index > 2){
            line_index++;
            column_index=0;
        }
    }
}

void SelectRobotMenu::init_challenge_creator(){
    challenge_creator = new ChallengeCreator();
    challenge_creator->create_maps("./data/levels");
}

void SelectRobotMenu::init_init_button(){
    std::pair<int, int> button_size(100,35);
    InitButton* init_challenge_button = new InitButton(
        "init_challenge_button",
        button_position,
        button_size
    );
    init_challenge_button->set_sprites(
        "./assets/sprites/buttons/button1.png",
        "./assets/sprites/buttons/button2.png"
    );
    init_challenge_button->set_selector(selector_field);
    init_challenge_button->set_challenge_creator(challenge_creator);
    init_challenge_button->activate();
    add_object(init_challenge_button);
}

void SelectRobotMenu::init_user(){
    UsersManage& users_manage = UsersManage::get_instance();
    current_user = users_manage.get_current_user();
}

void SelectRobotMenu::init_texts(){
    Engine::Field* title_robots = new Engine::Field(
        "title-robts",
        select_bot_position,
        std::make_pair(0,0)
    );
    title_robots->set_bold(true);
    title_robots->set_font("./assets/fonts/larabiefont-rg.ttf", 15);
    title_robots->set_text_per_line("selecione seu trator inteligente:", 0);
    title_robots->set_color(0xAAA, 0xAAA, 0xAAA, 0x00);
    add_object(title_robots);

    std::string username = current_user->get_name();
    Engine::Field* operator_name = new Engine::Field(
        "username",
        operator_name_position,
        std::make_pair(0,0)
    );
    operator_name->set_bold(true);
    operator_name->set_font("./assets/fonts/larabiefont-rg.ttf", 20);
    operator_name->set_text_per_line(username + ",", 0);
    operator_name->set_color(0xAAA, 0xAAA, 0xAAA, 0x00);
    add_object(operator_name);
}

void SelectRobotMenu::init_select_challenge_objects(){
    std::pair<int, int> button_size(40,40);
    next_challenge_button = new Engine::Button(
        "init_challenge_button",
        next_challenge_button_position,
        button_size
    );
    next_challenge_button->set_sprites(
        "./assets/sprites/buttons/next1.png",
        "./assets/sprites/buttons/next2.png"
    );
    next_challenge_button->activate();
    add_object(next_challenge_button);

    challenge_title = new Engine::Field(
        "challenge_title",
        challenge_title_position,
        std::make_pair(0,0)
    );
    challenge_title->set_bold(true);
    challenge_title->set_font("./assets/fonts/larabiefont-rg.ttf", 15);
    challenge_title->set_text_per_line(
        challenge_creator->get_challenge_title(challenge_index),
        0
    );
    challenge_title->set_color(0xAAA, 0xAAA, 0xAAA, 0x00);
    add_object(challenge_title);

    challenge_progress = new Engine::Field(
        "challenge-progress",
        challenge_progress_position,
        std::make_pair(0,0)
    );
    challenge_progress->set_bold(true);
    challenge_progress->set_font("./assets/fonts/larabiefont-rg.ttf", 15);
    challenge_progress->set_color(0xAAA, 0xAAA, 0xAAA, 0x00);
    add_object(challenge_progress);

    back_challenge_button = new Engine::Button(
        "init_challenge_button",
        back_challenge_button_position,
        button_size
    );
    back_challenge_button->set_sprites(
        "./assets/sprites/buttons/back1.png",
        "./assets/sprites/buttons/back2.png"
    );
    back_challenge_button->activate();
    add_object(back_challenge_button);
}

void SelectRobotMenu::draw(){
    for(auto object : objects){
        object->draw();
    }
    select_challenge();
}

void SelectRobotMenu::select_challenge(){
    if( 
        challenge_creator &&
        current_user
    ){
        update_challenge_info();
        if(
            next_challenge_button->get_pressed() &&
            challenge_index < challenge_creator->get_challenge_count() -1
        ){
            next_aux++;
        } else if(
            back_challenge_button->get_pressed() &&
            challenge_index > 0
        ){
            back_aux++;
        }

        if(next_aux >= 25){
            challenge_index++;
            next_aux = 0;
        }

        if(back_aux >= 25){
            challenge_index--;
            back_aux = 0;
        }
    }
}

void SelectRobotMenu::update_challenge_info(){
    std::string challenge_name = (
        challenge_creator->get_challenge_title(challenge_index)
    );

    challenge_title->set_text_per_line(challenge_name, 0);
    std::pair<int, int> user_progress = (
        current_user->get_challenge_progress(challenge_name)
    );

    challenge_progress->set_text_per_line(
        "realizados: " +
        std::to_string(user_progress.first) + 
        " | total: " +
        std::to_string(
            challenge_creator->get_challenge_maps_count(
                challenge_name
            )
        ),
        0
    );
}