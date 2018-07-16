#define Trabajo using
#define Practico namespace
#define III std;
#include <iostream>
#include <vector>
#include <algorithm>
#include <cstdio>
#include <cstdlib>
#include <chrono>
#include <tuple>
#include <cstdlib>
#include <fstream>
#include <string>
#include <utility>
#include <list>
#include <climits>
#include <assert.h>
#include <math.h>


Trabajo Practico III

typedef pair<int,int> par;
typedef pair<float,float> parFloat;
typedef vector<par> movimientos;
typedef tuple<int, string, par> mov;   // <id_jugador, tipo_mov, (dir, steps)> modelo el movimiento como par (dir, steps) para MOVIMIENTO y para PASE si es movimiento pongo (d, 0) porque no hay steps en realidad



par pos0 = make_pair(0,0);
par pos1 = make_pair(-1,-1);
par pos2 = make_pair(-1,0);
par pos3 = make_pair(-1,1);
par pos4 = make_pair(0,1);
par pos5 = make_pair(1,1);
par pos6 = make_pair(1,0);
par pos7 = make_pair(1,-1);
par pos8 = make_pair(0,-1);


movimientos moves ={pos0, pos1, pos2, pos3, pos4, pos5, pos6, pos7, pos8}; //pos0 no es direccion valida para la pelota

bool pertenece(int e, vector<int>& v){
    bool res = false;
    for(uint i = 0; i<v.size(); i++){
        if (e == v[i]){
            res = true;
            return res;
        }
    }
    return res;
}

bool sinParesRepetidos(vector<par> &pares){
    int i,j;
    for(uint k=0; k< pares.size(); k++){
        i = pares[k].first;
        j = pares[k].second;
        for(uint m=k+1; m < pares.size(); m++){
            if(i == pares[m].first && j==pares[m].second){
                return false;
            }
        }
    }
    return true;
}

mov buscarMov(int id,vector<mov> &moves){
    uint i=0;
    mov res;
    while(i<moves.size()){
        if(id == get<0>(moves[i])){
            res = moves[i];
        }
        i++;
    }
    return res;
}

bool pertenecePar(par &p, vector<par> &v){
    for(uint i =0; i<v.size(); i++){
        if(p.first == v[i].first && p.second == v[i].second){
            return true;
        }
    }
    return false;
}

bool is_neighbour(par &x, vector<par> &v){
    bool esVecino = false;
    for(uint i=0; i< v.size(); i++){
        if(abs(x.first - v[i].first)<2 && abs(x.second - v[i].second)<2 && (x.first != v[i].first && x.second != v[i].second)){
            esVecino = true;
        }
    }
    return esVecino;
}

vector<par> unir_vectores(vector<par> &a, vector<par> &b){
    for(uint i=0; i<b.size(); i++){
        a.push_back(b[i]);
    }
    return a;

}

class Ball{
public:

    Ball(bool tieneMovimiento=false): tieneMov(tieneMovimiento) {}

    void setMovement(par movimiento){
        // movement = (dir, steps)
        movement = movimiento;
        tieneMov = true;
    }

    void move(){
        if(tieneMov){
            if(movement.second > 0){
                par move = moves[movement.first];	//direccion de la pelota
                pel_i += 2*move.first;		//se mueve de a dos casilleros por paso
                pel_j += 2*move.second;
                movement = make_pair(movement.first, movement.second - 1);  //se reduce en 1 la cantidad de pasos
            }
        }
    }

    par finalPosition(){ //te da la posicion final de la pelota para el movimiento dado (podria tener que verificar si tiene o no movimiento la pelota)
        par move = moves[movement.first];
        int steps = movement.second;
        par res = make_pair(pel_i + 2*steps*move.first, pel_j + 2*steps*move.second);
        return res;
    }

    vector<par>& trajectory(){   //Me da el vector conteniendo la trayectoria de la pelota para un movimiento dado
        par move = moves[movement.first];
        int steps = 2*movement.second;
        vector<par> trayectoria;
        par p;
        for(int i=0; i<=steps; i++){
            p = make_pair(pel_i + i*move.first, pel_j + i*move.second);
            trayectoria.push_back(p);
        }
        return trayectoria;
    }

    void undoMove(){
        if (tieneMov){
            par move = moves[movement.first];
            pel_i -= 2*move.first;
            pel_j -= 2*move.second;
            movement = make_pair(movement.first, movement.second + 1);   //restaura la cantidad de pasos original
        }
    }

    void step_back_one(){   //solo retrocede un casillero en lugar de dos
        if(tieneMov){
            par move = moves[movement.first];
            pel_i -= move.first;
            pel_j -= move.second;
        }
    }

    void imprimirPelota(){

        cout << pel_i << " " << pel_j;

        if(tieneMov){
            cout << " " << movement.first << " "<< movement.second;
        }

        cout << endl;

    }

    void jugadorEnPosesion(par p){
        pel_i = p.first;
        pel_j = p.second;
    }

    bool tieneMovimiento(){
        return tieneMov;
    }

    par movimientoPelota(){
        return movement;
    }

    int posPel_i(){
        return pel_i;
    }

    int posPel_j(){
        return pel_j;
    }

private:
    par movement;
    bool tieneMov; //lo agrego para la funcion move
    int pel_i;
    int pel_j;
};

class Player{
public:

    Player(int player_id, float pQuite=0.5, bool tienePelota=false): id(player_id), p_quite(pQuite), hayPosesion(tienePelota) {}  //constructor de jugador

    void move(int direction){
        old_position = make_pair(jug_i, jug_j);
        jug_i += moves[direction].first;
        jug_j += moves[direction].second;
        moveBall();

    }

    void backwardMove(int direction){
        old_position = make_pair(jug_i, jug_j);
        jug_i -= moves[direction].first;
        jug_j -= moves[direction].second;
        moveBall();
    }

    void undoMove(){
        par p = make_pair(jug_i,jug_j);
        if(p != old_position){			//comparo el valor de la posicion actual con el de la posicion anterior si son iguales no hago nada.
            jug_i = old_position.first;
            jug_j = old_position.second;
            moveBall();
        }
        // solo guarda una jugada,
        // si quieren deshacer mas, se rompe a proposito
        //old_position = None; //Ver como representar esto en c++, quizas no es necesario porque a menos que haga un nuevo movimiento old_position sigue valiendo lo mismo

    }

    void moveBall(){   //el jugador se mueve con la pelota y por lo tanto la pelota tiene las mismas coordenadas que el jugador
        if(hayPosesion){						// if not self.ball is None:   -------ver esto!-----Creo que seria definir que el jugador tiene que tener la pelota
            par p = make_pair(jug_i, jug_j);
            ball.jugadorEnPosesion(p);
        }
    }

    void takeBall(Ball pelota){
        hayPosesion = true;
        ball = pelota;
        //ball.setMovement(None);  //para simular esto hago el if de abajo
        if(ball.tieneMovimiento()){
            par p = make_pair(0,0);
            ball.setMovement(p);   //Le anulo la cantidad de pasos restantes es como si hubiera llegado a destino
        }
        moveBall();
    }

    void imprimirJugador(){
        cout<< id << " " << jug_i << " " << jug_j << " ";
        if(hayPosesion){
            cout << "IN_POSETION" << endl;
        }else{
            cout << "FREE_PLAYER" << endl;
        }
    }

    int p_id(){
        return id;
    }



    bool tienePelota(){
        return hayPosesion;
    }
    int pos_i(){
        return jug_i;
    }

    int pos_j(){
        return jug_j;
    }

    void sinPelota(){
        hayPosesion = false;
    }

    Ball pelota(){
        return ball;
    }
    float quite(){
        return p_quite;
    }

    void posicionarJugador(par p){
        jug_i = p.first;
        jug_j = p.second;
    }

private:
    int id;
    float p_quite;
    par old_position;
    int jug_i;
    int jug_j;
    Ball ball;
    bool hayPosesion;   //agrego este bool para la funcion moveBall


};

typedef pair<vector<Player>, vector<Player> > statePlayer;   //(par(equipoA, equipoB), pos_i pelota, pos_j pelota, dir pelota, steps pelota)


class LogicalBoard{
public:

    LogicalBoard(int columnas, int filas, vector<par> team_1, vector<par> team_2, par marcador = make_pair(0,0)): score(marcador){   //Asumo el tipo de team_1 y team_2 contienen pares (p_id, p_quite) de cada jugador del equipo 1 y 2
        assert(((filas % 2) == 1) && (filas>=3));
        assert(((columnas % 2) == 0) && columnas>=2*filas);

        columns = columnas;
        rows = filas;
        hayPelotaLibre = false;
        hayEstadoAnteriorPlayer = false;
        hayEstadoAnteriorBall = false;
        par A,B;
        for(int i=0; i<3; i++){ //asumo que team_1 contiene pares (p_id, p_quite) de los 3 jugadores del equipo A
            team_A.push_back(Player(team_1[i].first, team_1[i].second));
            team_B.push_back(Player(team_2[i].first, team_2[i].second));
            goal_rows.push_back((int)(floor(rows/2) - 1 + i));
            A = make_pair(goal_rows[i],-1);
            B = make_pair(goal_rows[i], columns);
            goal_A.push_back(A);
            goal_B.push_back(B);
        }
    }
// moves = [(player_id, move_type, value)]
// value en [0, ..., 8] o (dir, steps) con dir en [1, ..., 8] y step en [0, ..., inf]

    bool posesion(char nombre){
        if(nombre == 'A'){
            for (int i = 0; i < 3; ++i) {
                if(team_A[i].tienePelota()) return true;
            }
        }
        else if(nombre == 'B'){
            for (int i = 0; i < 3; ++i) {
                if(team_B[i].tienePelota()) return true;
            }
        }
        else{
            return false;
        }

    }

    bool positionInBoard(int i, int j){
        return 0 <= i && i < rows && 0 <= j && j < columns;
    }

    bool isValidTeamMove(vector<Player> team, vector<mov> moves){
        bool valid = true;
        vector<int> player_ids;
        mov player_move;
        par posicion_jug;
        vector<par> ball_trajectory;
        vector<par> team_positions;
        bool estaEnArco, trajectory_in_board, trajectory_in_goal;
        // Exactamente un movimiento por jugador del equipo  //o sea len(moves) == 3
        for(uint i=0; i<team.size(); i++){
            player_ids.push_back(team[i].p_id());
        }
        vector<int> idEnMoves;
        for(uint i=0; i<moves.size(); i++){
            if(pertenece(get<0>(moves[i]), player_ids)){
                idEnMoves.push_back(get<0>(moves[i]));
            }
        }
        valid = valid && (moves.size() == team.size());
        valid = valid && (moves.size() == idEnMoves.size()); //verifique asi que los movimientos eran los de los jugadores del equipo y no hay ningun otro jugador en moves


        // muevo los jugadores
        if(valid){  //si no era valido hasta aca, no entro en el for porque se indefine player_move
            for (uint i=0; i<team.size(); i++){
                player_move = buscarMov(team[i].p_id(), moves);
                if (get<1>(player_move) == "MOVIMIENTO"){
                    team[i].move(get<2>(player_move).first);
                }else{
                    if(!team[i].tienePelota()){
                        valid = false;	//Quiere pasar la pelota pero no la tiene
                    }else{

                        // Mirar que el pase es válido: O sea que termina adentro de la cancha, en algún
                        // arco o cruza un arco (ya que va de a dos pasos por vez).
                        // Además, no puede ser más largo que M / 2
                        valid = valid && (get<2>(player_move).second <= rows / 2);

                        Ball ball;  //crea una pelota por defecto
                        posicion_jug = make_pair(team[i].pos_i(), team[i].pos_j());
                        ball.jugadorEnPosesion(posicion_jug);
                        cout << "is valid move ball" << endl;
                        //print ball
                        ball.imprimirPelota();
                        ball.setMovement(get<2>(player_move));
                        //print ball
                        ball.imprimirPelota();
                        ball_trajectory = ball.trajectory();
                        ////////////SACAR ESTA IMPRESION
                        for(uint i=0; i<ball_trajectory.size(); i++){
                            cout << "(" << ball_trajectory[i].first << ", " << ball_trajectory[i].second << ") ";
                        }
                        cout << endl;
                        cout << "is valid move ball" << endl;
                        trajectory_in_board = true;
                        trajectory_in_goal = false;
                        for(uint i =0; i<ball_trajectory.size(); i++){
                            if(!positionInBoard(ball_trajectory[i].first, ball_trajectory[i].second)){
                                trajectory_in_board = false;
                            }
                            if(pertenecePar(ball_trajectory[i], goal_A) || pertenecePar(ball_trajectory[i], goal_B)){
                                trajectory_in_goal = true;
                            }
                        }
                        valid = valid && (trajectory_in_board || trajectory_in_goal);
                    }
                }
                posicion_jug = make_pair(team[i].pos_i(), team[i].pos_j());
                team_positions.push_back(posicion_jug);
            }
        }
        // Dos jugadores del mismo equipo estan en la misma posicion
        valid = valid && sinParesRepetidos(team_positions);


        // Todos los jugadores deben estar dentro de la cancha
        for(uint i =0; i<team.size(); i++){
            //Tambien puede estar en un arco si es un jugador con pelota
            estaEnArco = team[i].tienePelota() && (pertenecePar(team_positions[i], goal_A) || pertenecePar(team_positions[i], goal_B));
            valid = valid && (positionInBoard(team[i].pos_i(), team[i].pos_j()) || estaEnArco);
        }
        // Deshago los movimientos
        for(uint i =0; i<moves.size(); i++){
            player_move = buscarMov(team[i].p_id(), moves);
            if (get<1>(player_move) == "MOVIMIENTO"){
                team[i].undoMove();
            }
        }

        return valid;

    }

    void makeTeamMove(vector<Player> team, vector<mov> moves){
        mov player_move;
        for(uint i =0; i<team.size(); i++){
            player_move = buscarMov(team[i].p_id(), moves);
            if (get<1>(player_move) == "MOVIMIENTO"){
                team[i].move(get<2>(player_move).first);
            }
            // Si el jugador pasó la pelota se setea la dirección y fuerza y se pierde
            // la posesión, luego el tablero detecta la pelota libre y la mueve en cada paso
            if (get<1>(player_move) == "PASE"){
                free_ball = team[i].pelota();
                free_ball.setMovement(get<2>(player_move));
                team[i].sinPelota(); // Ya no posee la pelota
                hayPelotaLibre = true; //La pelota ahora esta moviendose sola en la cancha
            }
        }
    }

    parFloat normalize(float prob_1, float prob_2){
        float total = prob_1 + prob_2;
        parFloat res = make_pair(prob_1/total, prob_2/total);  //no se si es un par
        return res;

    }

    void figthBall(Player p_ball, Player p_empty){
        float prob_ball = 1 - p_ball.quite(); // probabilidad de defensa
        float prob_empty = p_empty.quite(); // probabilidad de quite

        prob_empty = normalize(prob_ball, prob_empty).second;

        if ((((float) rand()) / 1) <= prob_empty){   //Hace que sacarle la pelota al otro jugador dependa del "azar" (***)
            p_empty.takeBall(p_ball.pelota());
            p_ball.sinPelota();
        }
    }

    void fairFightBall(Player p1, Player p2){

        float prob_p2 = normalize(p1.quite(), p2.quite()).second; // ambos usan la probabilidad de quite  (_, ***)
        float x = ((float) rand()) / 1;

        if (x < prob_p2){
            p2.takeBall(free_ball);
        }else{
            p1.takeBall(free_ball);
        }
        hayPelotaLibre = false;  //Un jugador agarro la pelota y ya no esta libre en la cancha
    }

    // Este metodo asume fuertemente que la pelota todavia no fue actualizada a su nueva posicion
    // y que la pelota esta libre.
    bool intercepted(Player curr_state_player, char team){
        bool result = true;

        // Buscar el estado anterior del jugador que recibo como parametro
        Player prev_state_player(curr_state_player.p_id(), curr_state_player.quite());
        if(team == 'A'){
            for(uint i = 0; i< (last_statePlayer.first).size(); i++){
                if(last_statePlayer.first[i].p_id() == curr_state_player.p_id()){//si es el jugador que buscaba
                    prev_state_player = last_statePlayer.first[i]; //Me guardo el estado anterior de ese jugador
                }
            }
        }else if (team == 'B'){
            for(uint i = 0; i< (last_statePlayer.second).size(); i++){
                if(last_statePlayer.second[i].p_id() == curr_state_player.p_id()){//si es el jugador que buscaba
                    prev_state_player = last_statePlayer.second[i];
                }
            }
        }
        // Si se movio no la intercepto
        result = result && prev_state_player.pos_i() == curr_state_player.pos_i() && prev_state_player.pos_j() == curr_state_player.pos_j();

        // Si esta en el camino la intercepta
        prev_state_player.backwardMove(free_ball.movimientoPelota().first);
        result = result && prev_state_player.pos_i() == free_ball.posPel_i() && prev_state_player.pos_j() == free_ball.posPel_j();
        prev_state_player.undoMove();

        return result;
    }

    char updateScore(){ //Devuelve el equipo goleado

        Ball ball = free_ball;

        if(!hayPelotaLibre){
            for(uint i=0; i<team_A.size(); i++){
                if(team_A[i].tienePelota()){
                    ball =  team_A[i].pelota();
                }
                if(team_B[i].tienePelota()){
                    ball =  team_B[i].pelota();
                }
            }
        }
        par pos_Pelota = make_pair(ball.posPel_i(), ball.posPel_j());
        if(pertenecePar(pos_Pelota, goal_A)){
            score = make_pair(score.first, score.second +1);
            return 'A'; 										//Devuelve el equipo goleado
        }else if(pertenecePar(pos_Pelota, goal_B)){
            score = make_pair(score.first+1, score.second);
            return 'B';
        }

        return 'N';    //No hubo gol (***) es un reemplazo de None

    }

    char makeMove(vector<mov> moves_A, vector<mov> moves_B){ //Me dice el arco goleado tras la jugada y actualiza el marcador, si no hubo gol devuelve N.
        assert(isValidTeamMove(team_A, moves_A));
        assert(isValidTeamMove(team_B, moves_B));

        getState();  //Antes de hacer algun movimiento cargo los valores del equipo actual en last_state

        makeTeamMove(team_A, moves_A);
        makeTeamMove(team_B, moves_B);
        vector<par> arcos = unir_vectores(goal_A, goal_B);


        //El balon se mueve en la dirección indicada por el ultimo pase
        if(hayPelotaLibre){
            par posPelota = make_pair(free_ball.posPel_i(), free_ball.posPel_j());
            // Mira si alguien interceptó la pelota
            vector<Player> intercepters;
            for(uint i=0; i<team_A.size(); i++){
                if(intercepted(team_A[i], 'A')){
                    intercepters.push_back(team_A[i]);
                }
                if(intercepted(team_B[i], 'B')){
                    intercepters.push_back(team_B[i]);
                }
            }
            assert(intercepters.size()<3);  //No puede haber mas de un jugador por equipo interceptando la pelota
            if (intercepters.size() == 1){
                intercepters[0].takeBall(free_ball);
                hayPelotaLibre = false;
            }else if (intercepters.size() == 2){
                fairFightBall(intercepters[0], intercepters[1]);
            }else{
                free_ball.move();  //No hubo ningun jugado interceptando la pelota, por lo tanto sigue su curso
                bool ball_in_board = positionInBoard(free_ball.posPel_i(), free_ball.posPel_j()); //Me fijo si despues del movimiento la pelota sigue en la cancha(sin inlcuir los arcos)
                if(ball_in_board){
                    // Si hay jugadores en ese casillero, entonces hay que ver si es uno
                    // solo entonces agarra la pelota y si son dos se la disputan
                    vector <Player> players_to_fight;
                    for(uint i=0; i<team_A.size(); i++){
                        if(team_A[i].pos_i() == free_ball.posPel_i() && team_A[i].pos_j() == free_ball.posPel_j()){
                            players_to_fight.push_back(team_A[i]);
                        }
                        if(team_B[i].pos_i() == free_ball.posPel_i() && team_B[i].pos_j() == free_ball.posPel_j()){
                            players_to_fight.push_back(team_B[i]);
                        }
                    }
                    if(players_to_fight.size() == 1){
                        players_to_fight[0].takeBall(free_ball);
                        hayPelotaLibre = false;
                    }else if(players_to_fight.size() == 2){
                        fairFightBall(players_to_fight[0], players_to_fight[1]);
                    }else if(is_neighbour(posPelota, arcos)){  //Si se verifico que era un movimiento valido no deberia pasar (**)
                        // Si la pelota no está en la cancha y es vecina del arco, entonces cruzo el arco
                        // y quedó atrapada en las redes, por lo que hay que volver un paso atrás.
                        free_ball.step_back_one();
                    }
                }

            }
        }else{ //La pelota no esta libre, o sea algun jugador la tiene
            //Si dos jugadores estan en el mismo casillero y uno tiene la pelota
            //Los mismos se disputan quien termina con la posesion.

            // Team A tiene la pelota
            bool alreadyFight = false;
            for(uint i=0; i<team_A.size(); i++){
                if(alreadyFight) break;
                if(team_A[i].tienePelota()){	//se fija para cada jugador de A si tiene la pelota
                    for(uint j=0; j<team_B.size(); j++){
                        if (team_A[i].pos_i() == team_B[j].pos_i() && team_A[i].pos_j() == team_B[j].pos_j()){
                            figthBall(team_A[i], team_B[j]);
                            alreadyFight = true;
                            break;
                        }
                    }
                }
            }
            if(!alreadyFight){  //Ninguno de A tenia la pelota o no habia ningun jugador de B en la misma posicion de un jugador de A
                // Team B tiene la pelota
                for(uint i=0; i<team_B.size(); i++){
                    if(alreadyFight) break;

                    if (team_B[i].tienePelota()){
                        for(uint j=0; j<team_A.size(); j++){
                            if (team_A[j].pos_i() == team_B[i].pos_i() && team_A[j].pos_j() == team_B[i].pos_j()){
                                figthBall(team_B[i], team_A[j]);
                                alreadyFight = true;
                                break;
                            }
                        }
                    }
                }
            }
        }
//Si alguien metió gol, tiene que actualizar el tablero y poner los equipos
//en las posiciones iniciales con el equipo al que le metieron gol sacando ....?
        return updateScore();

    }

    void getState(){
        if(hayPelotaLibre){
            last_stateBall = free_ball;
            hayEstadoAnteriorBall = true;
        }
        last_statePlayer = make_pair(team_A, team_B);
        hayEstadoAnteriorPlayer = true;
        last_score = score;

    }

    void undoMove(){
        if(hayEstadoAnteriorPlayer){
            team_A = last_statePlayer.first;
            team_B = last_statePlayer.second;
            hayEstadoAnteriorPlayer = false;	//No voy a poder hacer undoMove dos veces seguidas
        }
        if(hayEstadoAnteriorBall){
            free_ball = last_stateBall;
            hayEstadoAnteriorBall = false;
        }
        score = last_score;
    }


    char winner(){
        if (score.first > score.second){
            return 'A';
        }
        if(score.first < score.second){
            return 'B';
        }else{
            return 'E';    //sino devuelve Empate (En lugar de None)     (***)
        }
    }

//Asumo que las posiciones dentro de la cancha son validas y que siempre empieza el equipo llamado A a la izquierda de la cancha
    void startingPositions(vector<par> position_A, vector<par> position_B, char starting){
        //reseteo el estado anterior, empieza de cero
        hayEstadoAnteriorBall = false;
        hayEstadoAnteriorPlayer = false;
        // Saco la pelota del juego y pongo a los jugadores en su lugar
        for(uint i=0; i<team_A.size(); i++){
            team_A[i].sinPelota();
            team_B[i].sinPelota();

            team_A[i].posicionarJugador(position_A[i]);
            team_B[i].posicionarJugador(position_B[i]);

        }
        hayPelotaLibre = false;

        // le doy la pelota al jugador que saca y lo pongo en el centro; despues de un gol si el equipo B fue goleado, tiene la posesion de la pelota
        par pos_inicial_A = make_pair((int)(rows/2), (columns/2) - 1);
        par pos_inicial_B = make_pair((int)(rows/2), (columns/2));
        Ball b;
        if(starting == 'A'){
            team_A[0].posicionarJugador(pos_inicial_A);
            team_A[0].takeBall(b);

        }else if(starting == 'B'){
            team_B[0].posicionarJugador(pos_inicial_B);
            team_B[0].takeBall(b);
        }
    }

    void reset(vector<par> position_A, vector<par> position_B){  //reinicia el juego, siempre inicia el equipo A al comienzo del partido
        startingPositions(position_A, position_B, 'A');
        score = make_pair(0,0);
    }

    //PRE CONDICION QUE EL JUGADOR DEL EQUIPO DE ENTRADA TENGA LA PELOTA
    par jugador_con_pelota(char nombre){
        if(nombre == 'A'){
            for (int i = 0; i < 3; ++i) {
                if(team_A[i].tienePelota()){
                    return make_pair(team_A[i].pos_i(), team_A[i].pos_j());
                }
            }
        }
        else{
            for (int j = 0; j < 3; ++j) {
                if(team_B[j].tienePelota()){
                    return make_pair(team_B[j].pos_i(), team_B[j].pos_j());
                }
            }
        }
    }

    vector<par> getGoal(char team){
        if (team == 'A'){
            return goal_A;
        }else{
            return goal_B;
        }
    }

    vector<Player>& getitem(char team_name){
        if (team_name == 'A'){
            return team_A;
        }else if(team_name == 'B'){
            return team_B;
        }
    }

    bool pelota_libre(){
        return hayPelotaLibre;
    };

    Ball dame_pelota_libre(){
        return free_ball;
    };

    int& columnas(){
        return columns;
    };

    int& filas(){
        return rows;
    };

    par resultado(){
        return score;
    }

    par resultado_ant(){
        return last_score;
    }

private:
    par score;   //puntaje del partido
    vector<Player> team_A;
    vector<Player> team_B;
    int columns;
    int rows;
    vector<int> goal_rows; //filas del arco
    vector<par> goal_A;   //coordenadas del arco
    vector<par> goal_B;
    Ball free_ball;
    bool hayPelotaLibre;
    statePlayer last_statePlayer;
    bool hayEstadoAnteriorPlayer;
    Ball last_stateBall;
    bool hayEstadoAnteriorBall;
    par last_score;

};

class Team{
public:
    float distAlArco(LogicalBoard& t){
        float suma_total = 0;
        int dif_i, dif_j;
        int arco_i = (int)(floor(filas/2));
        int arco_j;
        vector<Player> ju = t.getitem(nombre);

        if(izq){
            arco_j = columnas-1;
        }
        else{
            arco_j = 0;
        }
        for (int i = 0; i < 3; ++i) {
            dif_i = pow((ju[i].pos_i() - arco_i), 2);
            dif_j = pow((ju[i].pos_j() - arco_j), 2);
            suma_total += sqrt(dif_i + dif_j);
        }
        suma_total = suma_total / (filas + columnas);
        return suma_total;
    };

    float distARival(LogicalBoard& t, par& rival){
        float suma_total = 0;
        vector<Player> equipoJ;
        equipoJ = t.getitem(nombre);
        for (int i = 0; i < 3; ++i) {
            suma_total = pow((equipoJ[i].pos_i() - rival.first), 2) + pow((equipoJ[i].pos_j() - rival.second), 2);
            suma_total = sqrt(suma_total);
        }
        suma_total = suma_total / (filas + columnas);
        return suma_total;
    };

    float distMinAPelota(LogicalBoard& t){ // toma la distancia minima de un jugador a la pelota
        float distancia = 0;
        int p_i, p_j, min;
        p_i = t.dame_pelota_libre().posPel_i();
        p_j = t.dame_pelota_libre().posPel_j();
        vector<Player> equipoJ;
        equipoJ = t.getitem(nombre);
        for (int i = 0; i < 3; ++i) {
            distancia = pow((equipoJ[i].pos_i() - p_i), 2) + pow((equipoJ[i].pos_j() - p_j), 2);
            distancia = sqrt(distancia);
            if(i == 0 || min > distancia){
                min = distancia;
            }
        }
        min = min / (filas + columnas);
        return min;
    }

    bool golAFavor(LogicalBoard& t){
        par res = t.resultado();
        par res_ant = t.resultado_ant();
        if (nombre == 'A') return (res.first > res_ant.first);
        return (res.second > res_ant.second);
    };

    bool golContra(LogicalBoard& t){
        par res = t.resultado();
        par res_ant = t.resultado_ant();
        if (nombre == 'B') return (res.first > res_ant.first);
        return (res.second > res_ant.second);
    }

    float distEntreJugadores(){

    }

    int yendoAlArco(LogicalBoard& t){
        Ball bocha = t.dame_pelota_libre();
        auto arco_rival = nombre == 'A'? t.getGoal('B'):t.getGoal('A');
        for (auto i = 0; i < 3; i++){
            if(pertenecePar(arco_rival[i], bocha.trajectory())) return 1;
        }
        return 0;
    }

    int anguloDeTiro(LogicalBoard& t){
        if (t.pelota_libre()) return 0;
        par pos = t.jugador_con_pelota(nombre);

        char rival = nombre == 'A'? 'B':'A';
        int cant_max_pasos = filas; // tengo M filas y solo se me permite patear con fuerza M/2 (la pelota recorre
        // como mucho una distancia M porque ).

        bool estoy_en_angulo = pos.first == t.getGoal(rival)[0].first ||
                               pos.first == t.getGoal(rival)[1].first ||
                               pos.first == t.getGoal(rival)[2].first;

        if (estoy_en_angulo && abs(pos.second - t.getGoal(rival)[0].second) <= cant_max_pasos) return 1;

        estoy_en_angulo |= abs(t.getGoal(rival)[0].first - pos.first) == abs(t.getGoal(rival)[0].second - pos.second) ||
                           abs(t.getGoal(rival)[1].first - pos.first) == abs(t.getGoal(rival)[1].second - pos.second) ||
                           abs(t.getGoal(rival)[2].first - pos.first) == abs(t.getGoal(rival)[2].second - pos.second);

        return int(estoy_en_angulo);
    }

    float puntuar_ofensiva(LogicalBoard& t){
        float puntaje_final = 0;
        //metodo que llama a la pos del team
        vector<Player> equipoJ;
        equipoJ = t.getitem(nombre);
        float esquiva = pesos[0] * (1-equipoJ[0].quite()) + pesos[1] * (1-equipoJ[1].quite()) +
                        pesos[2] * (1-equipoJ[2].quite());
        puntaje_final = pesos[3] * distAlArco(t) + esquiva + pesos[6] * (int)golAFavor(t) + pesos[8] * anguloDeTiro(t) +
                        pesos[9] * yendoAlArco(t);
        return puntaje_final;
    };

    float puntuar_defensiva(LogicalBoard& t){
        float puntaje_final = 0;
        vector<Player> equipoJ;
        equipoJ = t.getitem(nombre);
        char nom = 'A';
        if (nombre == 'A') nom = 'B';
        float quites = pesos[0] * equipoJ[0].quite() + pesos[1] * equipoJ[1].quite() + pesos[2] * equipoJ[2].quite();
        auto p = t.jugador_con_pelota(nom);
        puntaje_final = pesos[4] * distARival(t, p) + quites + pesos[7] * (int)golContra(t);
        return puntaje_final;
    };

    float puntuar_libre(LogicalBoard& t ){
        float puntaje_final = 0;
        puntaje_final = pesos[5] * distMinAPelota(t);
        return puntaje_final;
    };

    float tipo_de_movimientos(LogicalBoard& t){
        //equipo = tablero.posequipo;
        vector<mov> mejor_jugada;
        if(t.pelota_libre()){
            //auto p = make_pair(t.dame_pelota_libre().posPel_i(), t.dame_pelota_libre().posPel_j());
            //puntaje_final = puntuar_libre(p);
 //           mejor_jugada = generar_mov_libres(t);
        }
        else if(t.posesion(nombre)){
            // puntaje_final = puntuar_ofensiva(t); despues lo uso ?
  //          mejor_jugada = generar_mov_ofensivos(t);
        }
        else{
            //puntaje_final = puntuar_defensiva(t);
            //mejor_jugada = generar_mov_defensivo();
        }
    }

    vector<vector<mov>> generar_mov_libres(LogicalBoard& t){
        int ball_i = t.dame_pelota_libre().posPel_i();
        int ball_j = t.dame_pelota_libre().posPel_j();
        int i, j, resta;
        vector<Player> equipoJ;
        equipoJ = t.getitem(nombre);
        //tengo que hacer pelota menos jugador
        vector<vector<mov>> mov_equipo(3);
        mov movi0;
        for (int k = 0; k < 3; ++k) {
            i = equipoJ[k].pos_i();
            j = equipoJ[k].pos_j();
            i = ball_i - i;
            j = ball_j - j;
            movi0 = make_tuple(equipoJ[k].p_id(), "MOVIMIENTO", make_pair(0, 0));
            mov_equipo[k].push_back(movi0);
            if (i == 0){
                if(j > 0){
                    movi0 = make_tuple(equipoJ[k].p_id(), "MOVIMIENTO", make_pair(4, 0));
                    mov_equipo[k].push_back(movi0);
                    movi0 = make_tuple(equipoJ[k].p_id(), "MOVIMIENTO", make_pair(3, 0));
                    mov_equipo[k].push_back(movi0);
                    movi0 = make_tuple(equipoJ[k].p_id(), "MOVIMIENTO", make_pair(5, 0));
                    mov_equipo[k].push_back(movi0);
                    //AGREGAR LOS MOVIMIENTOS DIAGONALES EN ESTOS DE I O J IGUAL A 0
                }
                else{
                    movi0 = make_tuple(equipoJ[k].p_id(), "MOVIMIENTO", make_pair(8, 0));
                    mov_equipo[k].push_back(movi0);
                    movi0 = make_tuple(equipoJ[k].p_id(), "MOVIMIENTO", make_pair(1, 0));
                    mov_equipo[k].push_back(movi0);
                    movi0 = make_tuple(equipoJ[k].p_id(), "MOVIMIENTO", make_pair(7, 0));
                    mov_equipo[k].push_back(movi0);
                }
            }
            else if(j == 0){
                if(i > 0){
                    movi0 = make_tuple(equipoJ[k].p_id(), "MOVIMIENTO", make_pair(6, 0));
                    mov_equipo[k].push_back(movi0);
                    movi0 = make_tuple(equipoJ[k].p_id(), "MOVIMIENTO", make_pair(7, 0));
                    mov_equipo[k].push_back(movi0);
                    movi0 = make_tuple(equipoJ[k].p_id(), "MOVIMIENTO", make_pair(5, 0));
                    mov_equipo[k].push_back(movi0);
                }
                else{
                    movi0 = make_tuple(equipoJ[k].p_id(), "MOVIMIENTO", make_pair(2, 0));
                    mov_equipo[k].push_back(movi0);
                    movi0 = make_tuple(equipoJ[k].p_id(), "MOVIMIENTO", make_pair(1, 0));
                    mov_equipo[k].push_back(movi0);
                    movi0 = make_tuple(equipoJ[k].p_id(), "MOVIMIENTO", make_pair(3, 0));
                    mov_equipo[k].push_back(movi0);
                }
            }
            else{
                if(i > 0){
                    if(j > 0){
                        movi0 = make_tuple(equipoJ[k].p_id(), "MOVIMIENTO", make_pair(4, 0));
                        mov_equipo[k].push_back(movi0);
                        movi0 = make_tuple(equipoJ[k].p_id(), "MOVIMIENTO", make_pair(5, 0));
                        mov_equipo[k].push_back(movi0);
                        movi0 = make_tuple(equipoJ[k].p_id(), "MOVIMIENTO", make_pair(6, 0));
                        mov_equipo[k].push_back(movi0);
                    }
                    else{
                        movi0 = make_tuple(equipoJ[k].p_id(), "MOVIMIENTO", make_pair(6, 0));
                        mov_equipo[k].push_back(movi0);
                        movi0 = make_tuple(equipoJ[k].p_id(), "MOVIMIENTO", make_pair(7, 0));
                        mov_equipo[k].push_back(movi0);
                        movi0 = make_tuple(equipoJ[k].p_id(), "MOVIMIENTO", make_pair(8, 0));
                        mov_equipo[k].push_back(movi0);
                    }
                }
                else{
                    if(j > 0){
                        movi0 = make_tuple(equipoJ[k].p_id(), "MOVIMIENTO", make_pair(2, 0));
                        mov_equipo[k].push_back(movi0);
                        movi0 = make_tuple(equipoJ[k].p_id(), "MOVIMIENTO", make_pair(3, 0));
                        mov_equipo[k].push_back(movi0);
                        movi0 = make_tuple(equipoJ[k].p_id(), "MOVIMIENTO", make_pair(6, 0));
                        mov_equipo[k].push_back(movi0);
                    }
                    else{
                        movi0 = make_tuple(equipoJ[k].p_id(), "MOVIMIENTO", make_pair(1, 0));
                        mov_equipo[k].push_back(movi0);
                        movi0 = make_tuple(equipoJ[k].p_id(), "MOVIMIENTO", make_pair(2, 0));
                        mov_equipo[k].push_back(movi0);
                        movi0 = make_tuple(equipoJ[k].p_id(), "MOVIMIENTO", make_pair(8, 0));
                        mov_equipo[k].push_back(movi0);
                    }
                }
            }
        }
 //       vector<vector<mov>> v = crearMovValido(t, mov_equipo);
//        return v;
    }

    vector<vector<mov>> generar_mov_ofensivos(LogicalBoard& t){
        par arc_rival = t.getGoal('A')[1];
        if (nombre == 'A'){
            arc_rival = t.getGoal('B')[1];
        }
        vector<vector<mov>> mov_equipo(3);
        mov movi0;
        vector<Player> equipoJ;
        equipoJ = t.getitem(nombre);
        for (int i = 0; i < 3; ++i) {
            movi0 = make_tuple(equipoJ[i].p_id(), "MOVIMIENTO", make_pair(0, 0));
            mov_equipo[i].push_back(movi0);
            movi0 = make_tuple(equipoJ[i].p_id(), "MOVIMIENTO", make_pair(2, 0));
            mov_equipo[i].push_back(movi0);
            movi0 = make_tuple(equipoJ[i].p_id(), "MOVIMIENTO", make_pair(6, 0));
            mov_equipo[i].push_back(movi0);
            if(nombre == 'A'){
                movi0 = make_tuple(equipoJ[i].p_id(), "MOVIMIENTO", make_pair(3, 0));
                mov_equipo[i].push_back(movi0);
                movi0 = make_tuple(equipoJ[i].p_id(), "MOVIMIENTO", make_pair(4, 0));
                mov_equipo[i].push_back(movi0);
                movi0 = make_tuple(equipoJ[i].p_id(), "MOVIMIENTO", make_pair(5, 0));
                mov_equipo[i].push_back(movi0);
                if(equipoJ[i].tienePelota()){
                    int f_max = filas/2;
                    movi0 = make_tuple(equipoJ[i].p_id(), "PASE", make_pair(3, f_max));
                    mov_equipo[i].push_back(movi0);
                    movi0 = make_tuple(equipoJ[i].p_id(), "PASE", make_pair(4, f_max));
                    mov_equipo[i].push_back(movi0);
                    movi0 = make_tuple(equipoJ[i].p_id(), "PASE", make_pair(5, f_max));
                    mov_equipo[i].push_back(movi0);
                }
            }
            else {
                movi0 = make_tuple(equipoJ[i].p_id(), "MOVIMIENTO", make_pair(1, 0));
                mov_equipo[i].push_back(movi0);
                movi0 = make_tuple(equipoJ[i].p_id(), "MOVIMIENTO", make_pair(7, 0));
                mov_equipo[i].push_back(movi0);
                movi0 = make_tuple(equipoJ[i].p_id(), "MOVIMIENTO", make_pair(8, 0));
                mov_equipo[i].push_back(movi0);
                if(equipoJ[i].tienePelota()){
                    int f_max = filas/2;
                    movi0 = make_tuple(equipoJ[i].p_id(), "PASE", make_pair(1, f_max));
                    mov_equipo[i].push_back(movi0);
                    movi0 = make_tuple(equipoJ[i].p_id(), "PASE", make_pair(7, f_max));
                    mov_equipo[i].push_back(movi0);
                    movi0 = make_tuple(equipoJ[i].p_id(), "PASE", make_pair(8, f_max));
                    mov_equipo[i].push_back(movi0);
                }
            }
        }
     //   vector<vector<mov>> v = crearMovValido(t, mov_equipo);
   //     return v;
    }

    vector<vector<mov>> generar_mov_defensivos(LogicalBoard& t){
        par riv_ij, diff;
        nombre == 'A'? riv_ij = t.jugador_con_pelota('B') : riv_ij = t.jugador_con_pelota('A');
        vector<vector<mov>> mov_equipo(3);
        mov movi0;
        vector<Player> equipoJ;
        equipoJ = t.getitem(nombre);
//        vector<Player> ju_rival = t.getitem('A');
//        if(nombre == 'A') ju_rival = t.getitem('B');
//        int i =0;
//        for (i; !ju_rival[i].tienePelota(); ++i){}
        for (int k = 0; k < 3; ++k) {
            movi0 = make_tuple(equipoJ[k].p_id(), "MOVIMIENTO", make_pair(0, 0));
            mov_equipo[k].push_back(movi0);
            diff = make_pair(riv_ij.first - equipoJ[k].pos_i(), riv_ij.second - equipoJ[k].pos_j());
            if (diff.first == 0){
                if(diff.second > 0){
                    movi0 = make_tuple(equipoJ[k].p_id(), "MOVIMIENTO", make_pair(4, 0));
                    mov_equipo[k].push_back(movi0);
                    movi0 = make_tuple(equipoJ[k].p_id(), "MOVIMIENTO", make_pair(3, 0));
                    mov_equipo[k].push_back(movi0);
                    movi0 = make_tuple(equipoJ[k].p_id(), "MOVIMIENTO", make_pair(5, 0));
                    mov_equipo[k].push_back(movi0);
                    //AGREGAR LOS MOVIMIENTOS DIAGONALES EN ESTOS DE I O J IGUAL A 0
                }
                else{
                    movi0 = make_tuple(equipoJ[k].p_id(), "MOVIMIENTO", make_pair(8, 0));
                    mov_equipo[k].push_back(movi0);
                    movi0 = make_tuple(equipoJ[k].p_id(), "MOVIMIENTO", make_pair(1, 0));
                    mov_equipo[k].push_back(movi0);
                    movi0 = make_tuple(equipoJ[k].p_id(), "MOVIMIENTO", make_pair(7, 0));
                    mov_equipo[k].push_back(movi0);
                }
            }
            else if(diff.second == 0){
                if(diff.first > 0){
                    movi0 = make_tuple(equipoJ[k].p_id(), "MOVIMIENTO", make_pair(6, 0));
                    mov_equipo[k].push_back(movi0);
                    movi0 = make_tuple(equipoJ[k].p_id(), "MOVIMIENTO", make_pair(7, 0));
                    mov_equipo[k].push_back(movi0);
                    movi0 = make_tuple(equipoJ[k].p_id(), "MOVIMIENTO", make_pair(5, 0));
                    mov_equipo[k].push_back(movi0);
                }
                else{
                    movi0 = make_tuple(equipoJ[k].p_id(), "MOVIMIENTO", make_pair(2, 0));
                    mov_equipo[k].push_back(movi0);
                    movi0 = make_tuple(equipoJ[k].p_id(), "MOVIMIENTO", make_pair(1, 0));
                    mov_equipo[k].push_back(movi0);
                    movi0 = make_tuple(equipoJ[k].p_id(), "MOVIMIENTO", make_pair(3, 0));
                    mov_equipo[k].push_back(movi0);
                }
            }
            else{
                if(diff.first > 0){
                    if(diff.second > 0){
                        movi0 = make_tuple(equipoJ[k].p_id(), "MOVIMIENTO", make_pair(4, 0));
                        mov_equipo[k].push_back(movi0);
                        movi0 = make_tuple(equipoJ[k].p_id(), "MOVIMIENTO", make_pair(5, 0));
                        mov_equipo[k].push_back(movi0);
                        movi0 = make_tuple(equipoJ[k].p_id(), "MOVIMIENTO", make_pair(6, 0));
                        mov_equipo[k].push_back(movi0);
                    }
                    else{
                        movi0 = make_tuple(equipoJ[k].p_id(), "MOVIMIENTO", make_pair(6, 0));
                        mov_equipo[k].push_back(movi0);
                        movi0 = make_tuple(equipoJ[k].p_id(), "MOVIMIENTO", make_pair(7, 0));
                        mov_equipo[k].push_back(movi0);
                        movi0 = make_tuple(equipoJ[k].p_id(), "MOVIMIENTO", make_pair(8, 0));
                        mov_equipo[k].push_back(movi0);
                    }
                }
                else{
                    if(diff.second > 0){
                        movi0 = make_tuple(equipoJ[k].p_id(), "MOVIMIENTO", make_pair(2, 0));
                        mov_equipo[k].push_back(movi0);
                        movi0 = make_tuple(equipoJ[k].p_id(), "MOVIMIENTO", make_pair(3, 0));
                        mov_equipo[k].push_back(movi0);
                        movi0 = make_tuple(equipoJ[k].p_id(), "MOVIMIENTO", make_pair(6, 0));
                        mov_equipo[k].push_back(movi0);
                    }
                    else{
                        movi0 = make_tuple(equipoJ[k].p_id(), "MOVIMIENTO", make_pair(1, 0));
                        mov_equipo[k].push_back(movi0);
                        movi0 = make_tuple(equipoJ[k].p_id(), "MOVIMIENTO", make_pair(2, 0));
                        mov_equipo[k].push_back(movi0);
                        movi0 = make_tuple(equipoJ[k].p_id(), "MOVIMIENTO", make_pair(8, 0));
                        mov_equipo[k].push_back(movi0);
                    }
                }
            }
        }
    }


private:
    //podria cambiar la distancia de cada uno
    int filas;
    int columnas;
    int turnos;
    char nombre;
    bool izq;
    // de la posicion 0 a 2 estan los p.quite de cada jugador
    // en la posicion 3 esta la distancia al arco tiene que ser negativo (cuanto mas lejos peor)
    // en la posicion 4 esta la distancia al rival con pelota
    // en la posicion 5 esta la distancia a la pelota libre, (negativo).
    // en la posicion 6 es el peso de hacer un gol
    // en la posicion 7 es el peso de ser goleado (tiene que ser negativo)
    // en la posicion 8 es el peso del angulo del tiro del jugador con la pelota.
    vector<int> pesos;
};




int main(){
    float quite = 0.5;
    vector<par> team_1 = {make_pair(0, quite), make_pair(1, quite), make_pair(2, quite)};
    vector<par> team_2 = {make_pair(3, quite), make_pair(4, quite), make_pair(5, quite)};
    LogicalBoard tablero( 10, 5, team_1, team_2);

    vector<par> posA = {make_pair(1, 1), make_pair(2, 1), make_pair(3, 1)};
    vector<par> posB = {make_pair(1, 9), make_pair(2, 9), make_pair(3, 9)};
    tablero.reset(posA, posB);

    par m = make_pair(0, 0);
    vector<mov> jugada_B = {make_tuple(3, "MOVIMIENTO", m), make_tuple(4, "MOVIMIENTO", m), make_tuple(5, "MOVIMIENTO", m)};
    par m1 = make_pair(1, 1);
    par m2 = make_pair(3, 0);
    par m3 = make_pair(4, 0);
    vector<mov> jugada_A = {make_tuple(0, "PASE", m1), make_tuple(1, "MOVIMIENTO", m2), make_tuple(2, "MOVIMIENTO", m2)};

    tablero.makeMove(jugada_A, jugada_B);
    auto team1 = tablero.getitem('A');
    auto team2 = tablero.getitem('B');

    for (int i = 0; i < 3; ++i) {
        team1[i].imprimirJugador();
    }
    for (int i = 0; i < 3; ++i) {
        team2[i].imprimirJugador();

    }
    if(tablero.pelota_libre()){
        tablero.dame_pelota_libre().imprimirPelota();
    }




    srand(time(NULL));

    return 0;
}
