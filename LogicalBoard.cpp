#define Trabajo using
#define Practico namespace
#define III std;

#include <iostream>
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
#include <vector>

Trabajo Practico III

typedef pair<int, int> par;
typedef pair<float, float> parFloat;
typedef vector <par> movimientos;
typedef tuple<int, string, par> mov;   // <id_jugador, tipo_mov, (dir, steps)> modelo el movimiento como par (dir, steps) para MOVIMIENTO y para PASE si es movimiento pongo (d, 0) porque no hay steps en realidad



par pos0 = make_pair(0, 0);
par pos1 = make_pair(-1, -1);
par pos2 = make_pair(-1, 0);
par pos3 = make_pair(-1, 1);
par pos4 = make_pair(0, 1);
par pos5 = make_pair(1, 1);
par pos6 = make_pair(1, 0);
par pos7 = make_pair(1, -1);
par pos8 = make_pair(0, -1);


movimientos moves = {pos0, pos1, pos2, pos3, pos4, pos5, pos6, pos7, pos8}; //pos0 no es direccion valida para la pelota

bool pertenece(int e, vector<int> &v) {
    bool res = false;
    for (uint i = 0; i < v.size(); i++) {
        if (e == v[i]) {
            res = true;
            return res;
        }
    }
    return res;
}

bool sinParesRepetidos(vector <par> &pares) {
    int i, j;
    for (uint k = 0; k < pares.size(); k++) {
        i = pares[k].first;
        j = pares[k].second;
        for (uint m = k + 1; m < pares.size(); m++) {
            if (i == pares[m].first && j == pares[m].second) {
                return false;
            }
        }
    }
    return true;
}

mov buscarMov(int id, vector <mov> &moves) {
    uint i = 0;
    mov res;
    while (i < moves.size()) {
        if (id == get<0>(moves[i])) {
            res = moves[i];
        }
        i++;
    }
    return res;
}

bool pertenecePar(par &p, vector <par> &v) {
    for (uint i = 0; i < v.size(); i++) {
        if (p.first == v[i].first && p.second == v[i].second) {
            return true;
        }
    }
    return false;
}

bool is_neighbour(par &x, vector <par> &v) {
    bool esVecino = false;
    for (uint i = 0; i < v.size(); i++) {
        if (abs(x.first - v[i].first) < 2 && abs(x.second - v[i].second) < 2 &&
            (x.first != v[i].first && x.second != v[i].second)) {
            esVecino = true;
        }
    }
    return esVecino;
}

vector<par> unir_vectores(vector<par> a, vector<par> b) {
    for (uint i = 0; i < b.size(); i++) {
        a.push_back(b[i]);
    }
    return a;

}

float suma(vector<float>& v){
    float suma;
    for (int i = 0; i < v.size(); ++i) {
        suma += v[i];
    }
    return suma;
}

class Ball {
public:

    Ball(bool tieneMovimiento = false) : tieneMov(tieneMovimiento) {}

    void setMovement(par movimiento) {
        // movement = (dir, steps)
        movement = movimiento;
        tieneMov = true;
    }

    void move() {
        if (tieneMov) {
            if (movement.second > 0) {
                par move = moves[movement.first];    //direccion de la pelota
                pel_i += 2 * move.first;        //se mueve de a dos casilleros por paso
                pel_j += 2 * move.second;
                movement = make_pair(movement.first, movement.second - 1);  //se reduce en 1 la cantidad de pasos
                if(movement.second == 0){
                    tieneMov = false;
                }
            }
        }
    }

    par finalPosition() { //te da la posicion final de la pelota para el movimiento dado (podria tener que verificar si tiene o no movimiento la pelota)
        par move = moves[movement.first];
        int steps = movement.second;
        par res = make_pair(pel_i + 2 * steps * move.first, pel_j + 2 * steps * move.second);
        return res;
    }

    vector <par> trajectory() {   //Me da el vector conteniendo la trayectoria de la pelota para un movimiento dado
        par move = moves[movement.first];
        int steps = 2 * movement.second;
        vector<par> trayectoria;
        par p;
        for (int i = 0; i <= steps; i++) {
            p = make_pair(pel_i + i * move.first, pel_j + i * move.second);
            trayectoria.push_back(p);
        }
        return trayectoria;
    }

    void undoMove() {
        if (tieneMov) {
            par move = moves[movement.first];
            pel_i -= 2 * move.first;
            pel_j -= 2 * move.second;
            movement = make_pair(movement.first, movement.second + 1);   //restaura la cantidad de pasos original
        }
    }

    void step_back_one() {   //solo retrocede un casillero en lugar de dos
        int indice = movement.first;
        for (int i = 0; i < 4; ++i) {
            if(indice == 8) indice = 0;
            indice++;
        }
        par move = moves[indice];
        pel_i += move.first;
        pel_j += move.second;

    }

    void imprimirPelota() {

        cout << pel_i << " " << pel_j;

        if (tieneMov) {
            cout << " " << movement.first << " " << movement.second;
        }

        cout << endl;

    }

    void jugadorEnPosesion(par p) {
        pel_i = p.first;
        pel_j = p.second;
    }

    bool tieneMovimiento() {
        return tieneMov;
    }

    par movimientoPelota() {
        return movement;
    }

    int posPel_i() {
        return pel_i;
    }

    int posPel_j() {
        return pel_j;
    }

private:
    par movement;
    bool tieneMov; //lo agrego para la funcion move
    int pel_i;
    int pel_j;
};

class Player {
public:

    Player(int player_id, float pQuite = 0.5, bool tienePelota = false) : id(player_id), p_quite(pQuite), hayPosesion(
            tienePelota) {}  //constructor de jugador

    void move(int direction) {
        old_position = make_pair(jug_i, jug_j);
        jug_i += moves[direction].first;
        jug_j += moves[direction].second;
        moveBall();

    }

    void backwardMove(int direction) {
        old_position = make_pair(jug_i, jug_j);
        jug_i -= moves[direction].first;
        jug_j -= moves[direction].second;
        moveBall();
    }

    void undoMove() {
        par p = make_pair(jug_i, jug_j);
        if (p !=
            old_position) {            //comparo el valor de la posicion actual con el de la posicion anterior si son iguales no hago nada.
            jug_i = old_position.first;
            jug_j = old_position.second;
            moveBall();
        }
        // solo guarda una jugada,
        // si quieren deshacer mas, se rompe a proposito
        //old_position = None; //Ver como representar esto en c++, quizas no es necesario porque a menos que haga un nuevo movimiento old_position sigue valiendo lo mismo

    }

    void
    moveBall() {   //el jugador se mueve con la pelota y por lo tanto la pelota tiene las mismas coordenadas que el jugador
        if (hayPosesion) {                        // if not self.ball is None:   -------ver esto!-----Creo que seria definir que el jugador tiene que tener la pelota
            par p = make_pair(jug_i, jug_j);
            ball.jugadorEnPosesion(p);
        }
    }

    void takeBall(Ball pelota) {
        hayPosesion = true;
        ball = pelota;
        //ball.setMovement(None);  //para simular esto hago el if de abajo
        if (ball.tieneMovimiento()) {
            par p = make_pair(0, 0);
            ball.setMovement(p);   //Le anulo la cantidad de pasos restantes es como si hubiera llegado a destino
        }
        moveBall();
    }

    void imprimirJugador() {
        cout << id << " " << jug_i << " " << jug_j << " ";
        if (hayPosesion) {
            cout << "IN_POSETION" << endl;
        } else {
            cout << "FREE_PLAYER" << endl;
        }
    }

    int p_id() {
        return id;
    }


    bool tienePelota() {
        return hayPosesion;
    }

    int pos_i() {
        return jug_i;
    }

    int pos_j() {
        return jug_j;
    }

    void sinPelota() {
        hayPosesion = false;
    }

    Ball pelota() {
        return ball;
    }

    float quite() {
        return p_quite;
    }

    void posicionarJugador(par p) {
        jug_i = p.first;
        jug_j = p.second;
    }

    void cambia_quite(float q){
        p_quite = q;
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

typedef pair <vector<Player>, vector<Player>> statePlayer;   //(par(equipoA, equipoB), pos_i pelota, pos_j pelota, dir pelota, steps pelota)


class LogicalBoard {
public:

    LogicalBoard(int columnas, int filas,
                 vector <pair<int, float>> team_1,
                 vector <pair<int, float>> team_2,
                 par marcador = make_pair(0, 0)) : score(
            marcador) { //Asumo el tipo de team_1 y team_2 contienen pares (p_id, p_quite) de cada jugador del equipo 1 y 2

        assert(((filas % 2) == 1) && (filas >= 3));
        assert(((columnas % 2) == 0) && columnas >= 2 * filas);

        columns = columnas;
        rows = filas;
        hayPelotaLibre = false;
        hayEstadoAnteriorPlayer = false;
        hayEstadoAnteriorBall = false;
        par A, B;
        for (int i = 0; i < 3; i++) { //asumo que team_1 contiene pares (p_id, p_quite) de los 3 jugadores del equipo A
            team_A.push_back(Player(team_1[i].first, team_1[i].second));
            team_B.push_back(Player(team_2[i].first, team_2[i].second));
            goal_rows.push_back((int) (floor(rows / 2) - 1 + i));
            A = make_pair(goal_rows[i], -1);
            B = make_pair(goal_rows[i], columns);
            goal_A.push_back(A);
            goal_B.push_back(B);
        }
    }
// moves = [(player_id, move_type, value)]
// value en [0, ..., 8] o (dir, steps) con dir en [1, ..., 8] y step en [0, ..., inf]

    bool posesion(char nombre) {
        if (nombre == 'A') {
            for (int i = 0; i < 3; ++i) {
                if (team_A[i].tienePelota()) return true;
            }
        } else if (nombre == 'B') {
            for (int i = 0; i < 3; ++i) {
                if (team_B[i].tienePelota()) return true;
            }
        } else {
            return false;
        }

    }

    bool positionInBoard(int i, int j) {
        return 0 <= i && i < rows && 0 <= j && j < columns;
    }

    bool isValidTeamMove(vector <Player> team, vector <mov> moves) {
        bool valid = true;
        vector<int> player_ids;
        mov player_move;
        par posicion_jug;
        vector<par> ball_trajectory;
        vector<par> team_positions;
        bool estaEnArco, trajectory_in_board, trajectory_in_goal;
        // Exactamente un movimiento por jugador del equipo  //o sea len(moves) == 3
        for (uint i = 0; i < team.size(); i++) {
            player_ids.push_back(team[i].p_id());
        }
        vector<int> idEnMoves;
        for (uint i = 0; i < moves.size(); i++) {
            if (pertenece(get<0>(moves[i]), player_ids)) {
                idEnMoves.push_back(get<0>(moves[i]));
            }
        }
        valid = valid && (moves.size() == team.size());
        valid = valid && (moves.size() ==
                          idEnMoves.size()); //verifique asi que los movimientos eran los de los jugadores del equipo y no hay ningun otro jugador en moves


        // muevo los jugadores
        if (valid) {  //si no era valido hasta aca, no entro en el for porque se indefine player_move
            for (uint i = 0; i < team.size(); i++) {
                player_move = buscarMov(team[i].p_id(), moves);
                if (get<1>(player_move) == "MOVIMIENTO") {
                    team[i].move(get<2>(player_move).first);
                } else {
                    if (!team[i].tienePelota()) {
                        valid = false;    //Quiere pasar la pelota pero no la tiene
                    } else {

                        // Mirar que el pase es válido: O sea que termina adentro de la cancha, en algún
                        // arco o cruza un arco (ya que va de a dos pasos por vez).
                        // Además, no puede ser más largo que M / 2
                        valid = valid && (get<2>(player_move).second <= rows / 2);

                        Ball ball;  //crea una pelota por defecto
                        posicion_jug = make_pair(team[i].pos_i(), team[i].pos_j());
                        ball.jugadorEnPosesion(posicion_jug);
                        ball.setMovement(get<2>(player_move));
                        ball_trajectory = ball.trajectory();
                        trajectory_in_board = true;
                        trajectory_in_goal = false;
                        for (uint i = 0; i < ball_trajectory.size(); i++) {
                            if (!positionInBoard(ball_trajectory[i].first, ball_trajectory[i].second)) {
                                trajectory_in_board = false;
                            }
                            if (pertenecePar(ball_trajectory[i], goal_A) || pertenecePar(ball_trajectory[i], goal_B)) {
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
        for (uint i = 0; i < team.size(); i++) {
            //Tambien puede estar en un arco si es un jugador con pelota
            estaEnArco = team[i].tienePelota() &&
                         (pertenecePar(team_positions[i], goal_A) || pertenecePar(team_positions[i], goal_B));
            valid = valid && (positionInBoard(team[i].pos_i(), team[i].pos_j()) || estaEnArco);
        }
        // Deshago los movimientos
        for (uint i = 0; i < moves.size(); i++) {
            player_move = buscarMov(team[i].p_id(), moves);
            if (get<1>(player_move) == "MOVIMIENTO") {
                team[i].undoMove();
            }
        }

        return valid;

    }


    void makeTeamMove(vector<Player> &team, vector<mov> &moves) {
        mov player_move;
        for (uint i = 0; i < team.size(); i++) {
            player_move = buscarMov(team[i].p_id(), moves);
            if (get<1>(player_move) == "MOVIMIENTO") {
                team[i].move(get<2>(player_move).first);
            }
            // Si el jugador pasó la pelota se setea la dirección y fuerza y se pierde
            // la posesión, luego el tablero detecta la pelota libre y la mueve en cada paso
            if (get<1>(player_move) == "PASE") {
                free_ball = team[i].pelota();
                free_ball.setMovement(get<2>(player_move));
                team[i].sinPelota(); // Ya no posee la pelota
                hayPelotaLibre = true; //La pelota ahora esta moviendose sola en la cancha
            }
        }
    }

    parFloat normalize(float prob_1, float prob_2) {
        float total = prob_1 + prob_2;
        parFloat res = make_pair(prob_1 / total, prob_2 / total);  //no se si es un par
        return res;

    }

    void figthBall(Player &p_ball, Player &p_empty) {
        float prob_ball = 1 - p_ball.quite(); // probabilidad de defensa
        float prob_empty = p_empty.quite(); // probabilidad de quite

        prob_empty = normalize(prob_ball, prob_empty).second;
        //auto rando = rand() / (float) RAND_MAX;

        if ((rand() / (float) RAND_MAX) <=
            prob_empty) {   //Hace que sacarle la pelota al otro jugador dependa del "azar" (***)
            p_empty.takeBall(p_ball.pelota());
            p_ball.sinPelota();
        }
    }

    void fairFightBall(Player &p1, Player &p2) {

        float prob_p2 = normalize(p1.quite(), p2.quite()).second; // ambos usan la probabilidad de quite  (_, ***)
        float x = rand() / (float) RAND_MAX;

        if (x < prob_p2) {
            p2.takeBall(free_ball);
        } else {
            p1.takeBall(free_ball);
        }
        hayPelotaLibre = false;  //Un jugador agarro la pelota y ya no esta libre en la cancha
    }

    // Este metodo asume fuertemente que la pelota todavia no fue actualizada a su nueva posicion
    // y que la pelota esta libre.
    bool intercepted(Player curr_state_player, char team) {
        bool result = true;

        // Buscar el estado anterior del jugador que recibo como parametro
        Player prev_state_player(curr_state_player.p_id(), curr_state_player.quite());
        if (team == 'A') {
            for (uint i = 0; i < (last_statePlayer.first).size(); i++) {
                if (last_statePlayer.first[i].p_id() == curr_state_player.p_id()) {//si es el jugador que buscaba
                    prev_state_player = last_statePlayer.first[i]; //Me guardo el estado anterior de ese jugador
                }
            }
        } else if (team == 'B') {
            for (uint i = 0; i < (last_statePlayer.second).size(); i++) {
                if (last_statePlayer.second[i].p_id() == curr_state_player.p_id()) {//si es el jugador que buscaba
                    prev_state_player = last_statePlayer.second[i];
                }
            }
        }
        // Si se movio no la intercepto
        result = result && prev_state_player.pos_i() == curr_state_player.pos_i() &&
                 prev_state_player.pos_j() == curr_state_player.pos_j();

        // Si esta en el camino la intercepta
        prev_state_player.backwardMove(free_ball.movimientoPelota().first);
        result = result && prev_state_player.pos_i() == free_ball.posPel_i() &&
                 prev_state_player.pos_j() == free_ball.posPel_j();
        prev_state_player.undoMove();

        return result;
    }


//Asumo que las posiciones dentro de la cancha son validas y que siempre empieza el equipo llamado A a la izquierda de la cancha
    void startingPositions(vector<par> position_A, vector<par> position_B, char starting) {
        //reseteo el estado anterior, empieza de cero
        posicionesIniciales_A = position_A;
        posicionesIniciales_B = position_B;
        // Saco la pelota del juego y pongo a los jugadores en su lugar
        for (uint i = 0; i < team_A.size(); i++) {
            team_A[i].sinPelota();
            team_B[i].sinPelota();

            team_A[i].posicionarJugador(position_A[i]);
            team_B[i].posicionarJugador(position_B[i]);

        }
        hayPelotaLibre = false;

        // le doy la pelota al jugador que saca y lo pongo en el centro; despues de un gol si el equipo B fue goleado, tiene la posesion de la pelota
        par pos_inicial_A = make_pair((int) (rows / 2), (columns / 2) - 1);
        par pos_inicial_B = make_pair((int) (rows / 2), (columns / 2));
        Ball b;
        if (starting == 'A') {
            team_A[0].posicionarJugador(pos_inicial_A);
            team_A[0].takeBall(b);

        } else if (starting == 'B') {
            team_B[0].posicionarJugador(pos_inicial_B);
            team_B[0].takeBall(b);
        }
    }

    char updateScore() { //Devuelve el equipo goleado

        Ball ball = free_ball;
        char res;
        if (!hayPelotaLibre) {
            for (uint i = 0; i < team_A.size(); i++) {
                if (team_A[i].tienePelota()) {
                    ball = team_A[i].pelota();
                }
                if (team_B[i].tienePelota()) {
                    ball = team_B[i].pelota();
                }
            }
        }
        par pos_Pelota = make_pair(ball.posPel_i(), ball.posPel_j());
        if (pertenecePar(pos_Pelota, goal_A)) {
            score = make_pair(score.first, score.second + 1);
            res = 'A';                                //Devuelve el equipo goleado
            startingPositions(posicionesIniciales_A, posicionesIniciales_B, res);
        } else if (pertenecePar(pos_Pelota, goal_B)) {
            score = make_pair(score.first + 1, score.second);
            res = 'B';
            startingPositions(posicionesIniciales_A, posicionesIniciales_B, res);
        }
        res = 'N';    //No hubo gol (***) es un reemplazo de None

        return res;
    }


    char makeMove(vector <mov> moves_A,
                  vector <mov> moves_B) { //Me dice el arco goleado tras la jugada y actualiza el marcador, si no hubo gol devuelve N.
        assert(isValidTeamMove(team_A, moves_A));
        assert(isValidTeamMove(team_B, moves_B));

        getState();  //Antes de hacer algun movimiento cargo los valores del equipo actual en last_state

        makeTeamMove(team_A, moves_A);
        makeTeamMove(team_B, moves_B);
        vector<par> arcos = unir_vectores(goal_A, goal_B);


        //El balon se mueve en la dirección indicada por el ultimo pase
        if (hayPelotaLibre) {
            par posPelota = make_pair(free_ball.posPel_i(), free_ball.posPel_j());
            // Mira si alguien interceptó la pelota
            vector<int> intercepters;
            vector<char> eqnom;

            for (int i = 0; i < team_A.size(); i++) {
                if (intercepted(team_A[i], 'A')) {
                    intercepters.push_back(i);
                    eqnom.push_back('A');
                }
                if (intercepted(team_B[i], 'B')) {
                    intercepters.push_back(i);
                    eqnom.push_back('B');
                }
            }
            assert(intercepters.size() < 3);  //No puede haber mas de un jugador por equipo interceptando la pelota
            if (intercepters.size() == 1) {
                //DEBUGUEAAARRRRR
                if(eqnom[0] == 'A'){
                    team_A[intercepters[0]].takeBall(free_ball);
                }
                else{
                    team_B[intercepters[0]].takeBall(free_ball);
                }
                hayPelotaLibre = false;
            } else if (intercepters.size() == 2) {
                if(eqnom[0] == 'A'){
                    fairFightBall(team_A[intercepters[0]], team_B[intercepters[1]]);
                }
                else{
                    fairFightBall(team_A[intercepters[1]], team_B[intercepters[0]]);
                }

            } else {
                free_ball.move();  //No hubo ningun jugado interceptando la pelota, por lo tanto sigue su curso
                bool ball_in_board = positionInBoard(free_ball.posPel_i(),
                                                     free_ball.posPel_j()); //Me fijo si despues del movimiento la pelota sigue en la cancha(sin inlcuir los arcos)
                if (ball_in_board) {
                    // Si hay jugadores en ese casillero, entonces hay que ver si es uno
                    // solo entonces agarra la pelota y si son dos se la disputan
                    vector<par> players_to_fight;
                    for (uint i = 0; i < team_A.size(); i++) {
                        if (team_A[i].pos_i() == free_ball.posPel_i() && team_A[i].pos_j() == free_ball.posPel_j()) {
                            players_to_fight.push_back(make_pair(0, i));
                        }
                        if (team_B[i].pos_i() == free_ball.posPel_i() && team_B[i].pos_j() == free_ball.posPel_j()) {
                            players_to_fight.push_back(make_pair(1, i));
                        }
                    }
                    if (players_to_fight.size() == 1) {
                        if (players_to_fight[0].first == 0){
                            team_A[players_to_fight[0].second].takeBall(free_ball);
                        }
                        else{
                            team_B[players_to_fight[0].second].takeBall(free_ball);
                        }
                        hayPelotaLibre = false;
                    } else if (players_to_fight.size() == 2){
                        if(players_to_fight[0].first == 0){
                            fairFightBall(team_A[players_to_fight[0].second], team_B[players_to_fight[1].second]);
                        }
                        else{
                            fairFightBall(team_A[players_to_fight[1].second], team_B[players_to_fight[0].second]);
                        }
                    }

                }
                else if (is_neighbour(posPelota,
                                      arcos)) {  //Si se verifico que era un movimiento valido no deberia pasar (**)
                    // Si la pelota no está en la cancha y es vecina del arco, entonces cruzo el arco
                    // y quedó atrapada en las redes, por lo que hay que volver un paso atrás.
                    free_ball.step_back_one();
                }

            }
        } else { //La pelota no esta libre, o sea algun jugador la tiene
            //Si dos jugadores estan en el mismo casillero y uno tiene la pelota
            //Los mismos se disputan quien termina con la posesion.

            // Team A tiene la pelota
            bool alreadyFight = false;
            for (uint i = 0; i < team_A.size(); i++) {
                if (alreadyFight) break;
                if (team_A[i].tienePelota()) {    //se fija para cada jugador de A si tiene la pelota
                    for (uint j = 0; j < team_B.size(); j++) {
                        if (team_A[i].pos_i() == team_B[j].pos_i() && team_A[i].pos_j() == team_B[j].pos_j()) {
                            figthBall(team_A[i], team_B[j]);
                            alreadyFight = true;
                            break;
                        }
                    }
                }
            }
            if (!alreadyFight) {  //Ninguno de A tenia la pelota o no habia ningun jugador de B en la misma posicion de un jugador de A
                // Team B tiene la pelota
                for (uint i = 0; i < team_B.size(); i++) {
                    if (alreadyFight) break;

                    if (team_B[i].tienePelota()) {
                        for (uint j = 0; j < team_A.size(); j++) {
                            if (team_A[j].pos_i() == team_B[i].pos_i() && team_A[j].pos_j() == team_B[i].pos_j()) {
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

    void getState() {
        if (hayPelotaLibre) {

            last_stateBall = free_ball;
            hayEstadoAnteriorBall = true;
        }
        estabaLibre = hayPelotaLibre;
        last_statePlayer = make_pair(team_A, team_B);
        hayEstadoAnteriorPlayer = true;
        last_score = score;
    }

    void undoMove() {
        if (hayEstadoAnteriorPlayer) {
            team_A = last_statePlayer.first;
            team_B = last_statePlayer.second;
            hayEstadoAnteriorPlayer = false;    //No voy a poder hacer undoMove dos veces seguidas
        }
        if (hayEstadoAnteriorBall) {
            free_ball = last_stateBall;
            hayEstadoAnteriorBall = false;
        }
        hayPelotaLibre = estabaLibre;
        score = last_score;
    }


    char winner() {
        if (score.first > score.second) {
            return 'A';
        }
        if (score.first < score.second) {
            return 'B';
        } else {
            return 'E';    //sino devuelve Empate (En lugar de None)     (***)
        }
    }

    void reset(vector <par> position_A,
               vector <par> position_B) {  //reinicia el juego, siempre inicia el equipo A al comienzo del partido
        startingPositions(position_A, position_B, 'A');
        hayEstadoAnteriorBall = false;
        hayEstadoAnteriorPlayer = false;

        score = make_pair(0, 0);
    }

    //PRE CONDICION QUE EL JUGADOR DEL EQUIPO DE ENTRADA TENGA LA PELOTA
    par jugador_con_pelota(char nombre) {
        if (nombre == 'A') {
            for (int i = 0; i < 3; ++i) {
                if (team_A[i].tienePelota()) {
                    return make_pair(team_A[i].pos_i(), team_A[i].pos_j());
                }
            }
        } else {
            for (int j = 0; j < 3; ++j) {
                if (team_B[j].tienePelota()) {
                    return make_pair(team_B[j].pos_i(), team_B[j].pos_j());
                }
            }
        }
    }
//Precondicion requiere que un jugador del equipo tenga posesion de la pelota

    Player quienTienePelota(char nombre) {
        if (nombre == 'A') {
            for (int i = 0; i < 3; ++i) {
                if (team_A[i].tienePelota()) {
                    return team_A[i];
                }
            }
        } else {
            for (int i = 0; i < 3; ++i) {
                if (team_B[i].tienePelota()) {
                    return team_B[i];
                }
            }
        }
    }


    vector <par> getGoal(char team) {
        if (team == 'A') {
            return goal_A;
        } else {
            return goal_B;
        }
    }

    vector <Player> &getitem(char team_name) {
        if (team_name == 'A') {
            return team_A;
        } else if (team_name == 'B') {
            return team_B;
        }
    }

    bool pelota_libre() {
        return hayPelotaLibre;
    };

    Ball dame_pelota_libre() {
        return free_ball;
    };

    int &columnas() {
        return columns;
    };

    int &filas() {
        return rows;
    };

    par resultado() {
        return score;
    }

    par resultado_ant() {
        return last_score;
    }

    void cambiarPesos(vector<float> quites, char name){
        if (name == 'A'){
            for (int i = 0; i < 3; ++i) {
                team_A[i].cambia_quite(quites[i]);
            }
        } else if(name == 'B'){
            for (int i = 0; i < 3; ++i) {
                team_B[i].cambia_quite(quites[i]);
            }
        }
    }

private:
    par score;   //puntaje del partido
    vector <Player> team_A;
    vector <Player> team_B;
    int columns;
    int rows;
    vector<int> goal_rows; //filas del arco
    vector <par> goal_A;   //coordenadas del arco
    vector <par> goal_B;
    Ball free_ball;
    bool hayPelotaLibre;
    statePlayer last_statePlayer;
    bool hayEstadoAnteriorPlayer;
    Ball last_stateBall;
    bool hayEstadoAnteriorBall;
    vector <par> posicionesIniciales_A;
    vector <par> posicionesIniciales_B;
    par last_score;
    bool estabaLibre;
};


class Team {
public:

    Team(int filas, int columnas, char nombre, vector<float> pesos, int turnos) : filas(filas), columnas(columnas),
                                                                                  nombre(nombre), pesos(pesos),
                                                                                  turnos(turnos) {
        izq = nombre == 'A';
    }

    float distAlArco(LogicalBoard &t) {
        float suma_total = 0;
        int dif_i, dif_j;
        int arco_i = (int) (floor(filas / 2));
        int arco_j;
        vector<Player> ju = t.getitem(nombre);

        if (nombre == 'A') {
            //arco_j = columnas - 1;
            arco_j = columnas;
        } else {
            //arco_j = 0;
            arco_j = -1;
        }
        for (int i = 0; i < 3; ++i) {
            dif_i = pow((ju[i].pos_i() - arco_i), 2);
            dif_j = pow((ju[i].pos_j() - arco_j), 2);
            suma_total += sqrt(dif_i + dif_j);
        }
        suma_total = suma_total / (3 * (filas + columnas));
        return suma_total;
    };

    float distARival(LogicalBoard &t, par &rival, int j) {
        float tmp = 0;
        float suma_total = 0;
        vector<Player> equipoJ;
        equipoJ = t.getitem(nombre);
        for (int i = 0; i < 3; ++i) {
            if (j != i) {
                tmp = pow((equipoJ[i].pos_i() - rival.first), 2) + pow((equipoJ[i].pos_j() - rival.second), 2);
                tmp = sqrt(tmp);
                suma_total += tmp;
            }
        }
        suma_total = suma_total / (2*(filas + columnas));
        return suma_total;
    };

    float distMinAPelota(LogicalBoard &t) { // toma la distancia minima de un jugador a la pelota
        float distancia = 0;
        int p_i, p_j;
        float min;
        p_i = t.dame_pelota_libre().posPel_i();
        p_j = t.dame_pelota_libre().posPel_j();
        vector<Player> equipoJ;
        equipoJ = t.getitem(nombre);
        for (int i = 0; i < 3; ++i) {
            distancia = pow((equipoJ[i].pos_i() - p_i), 2) + pow((equipoJ[i].pos_j() - p_j), 2);
            distancia = sqrt(distancia);
            if (i == 0 || min > distancia) {
                min = distancia;
            }
        }
        min = min / (filas + columnas);
        return min;
    }

    bool golAFavor(LogicalBoard &t) {
        par res = t.resultado();
        par res_ant = t.resultado_ant();
        if (nombre == 'A') return (res.first > res_ant.first);
        return (res.second > res_ant.second);
    };

    bool golContra(LogicalBoard &t) {
        par res = t.resultado();
        par res_ant = t.resultado_ant();
        if (nombre == 'B') return (res.first > res_ant.first);
        return (res.second > res_ant.second);
    }

    int yendoAlArco(LogicalBoard &t) {
        vector<par> trayectoria = t.dame_pelota_libre().trajectory();

        auto arco_rival = nombre == 'A' ? t.getGoal('B') : t.getGoal('A');
        for (auto i = 0; i < 3; i++) {
            if (pertenecePar(arco_rival[i], trayectoria)) return 1;
        }
        return 0;
    }

    int equipoEnTrayectoria(LogicalBoard &t) {
        vector<par> trayectoria = t.dame_pelota_libre().trajectory();

        char rival = nombre == 'A' ? 'B' : 'A';
        int i_ju, j_ju, i_ri, j_ri;
        for (auto i = 0; i < trayectoria.size(); i++) {
            int i_ju = t.getitem(nombre)[i].pos_i();
            int j_ju = t.getitem(nombre)[i].pos_j();
            if (trayectoria[i].first == i_ju && trayectoria[i].second == j_ju) return 1;
            int i_ri = t.getitem(rival)[i].pos_i();
            int j_ri = t.getitem(rival)[i].pos_j();
            if (trayectoria[i].first == i_ri && trayectoria[i].second == j_ri) return (-1);
        }

        return 0;
    }

    int anguloDeTiro(LogicalBoard &t) {
        par pos = t.jugador_con_pelota(nombre);

        char rival = nombre == 'A' ? 'B' : 'A';
        int cant_max_pasos = filas; // tengo M filas y solo se me permite patear con fuerza M/2 (la pelota recorre
        // como mucho una distancia M porque).

        bool estoy_en_angulo = pos.first == t.getGoal(rival)[0].first ||
                               pos.first == t.getGoal(rival)[1].first ||
                               pos.first == t.getGoal(rival)[2].first;

        if (estoy_en_angulo && abs(pos.second - t.getGoal(rival)[0].second) <= cant_max_pasos) return 1;

        estoy_en_angulo |= abs(t.getGoal(rival)[0].first - pos.first) == abs(t.getGoal(rival)[0].second - pos.second) ||
                           abs(t.getGoal(rival)[1].first - pos.first) == abs(t.getGoal(rival)[1].second - pos.second) ||
                           abs(t.getGoal(rival)[2].first - pos.first) == abs(t.getGoal(rival)[2].second - pos.second);

        return int(estoy_en_angulo);
    }

    float puntuar_ofensiva(LogicalBoard &t) {
        float puntaje_final = 0;
        vector<Player> equipoJ;
        equipoJ = t.getitem(nombre);
        puntaje_final += (1 -equipoJ[0].quite()) * (int)equipoJ[0].tienePelota();
        puntaje_final += pesos[3] * distAlArco(t) + pesos[4] * anguloDeTiro(t);
        puntaje_final += pesos[5] * (int) golContra(t);
        return puntaje_final;
    };

    float puntuar_defensiva(LogicalBoard &t) {
        float puntaje_final = 0;
        vector<Player> equipoJ;
        equipoJ = t.getitem(nombre);
        char rival = nombre == 'B' ? 'A' : 'B';
        puntaje_final += equipoJ[1].quite() + equipoJ[2].quite();
        auto p = t.jugador_con_pelota(rival);
        puntaje_final += pesos[7] * distARival(t, p, 0);
        puntaje_final += + pesos[6] * (int) golAFavor(t);
        return puntaje_final;
    };

    float puntuar_libre(LogicalBoard &t) {
        float puntaje_final = 0;
        puntaje_final = pesos[8] * distMinAPelota(t) + pesos[9] * yendoAlArco(t) + pesos[10] * equipoEnTrayectoria(t);
        return puntaje_final;
    };

    vector <mov> generarJugada(LogicalBoard &t) {
        vector<vector<mov>> jugadas;
        vector<mov> mejor_jugada;
        if (t.pelota_libre()) {
            jugadas = generar_mov_libres(t);
            mejor_jugada = elegirMov(t, jugadas);

        } else if (t.posesion(nombre)) {
            jugadas = generar_mov_ofensivos(t);
            mejor_jugada = elegirMov(t, jugadas);
        } else {
            jugadas = generar_mov_defensivos(t);
            mejor_jugada = elegirMov(t, jugadas);
        }
        return mejor_jugada;
    }

    vector <vector<mov>> generar_mov_libres(LogicalBoard &t) {
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
            if (i == 0) {
                if (j > 0) {
                    movi0 = make_tuple(equipoJ[k].p_id(), "MOVIMIENTO", make_pair(4, 0));
                    mov_equipo[k].push_back(movi0);
                    movi0 = make_tuple(equipoJ[k].p_id(), "MOVIMIENTO", make_pair(3, 0));
                    mov_equipo[k].push_back(movi0);
                    movi0 = make_tuple(equipoJ[k].p_id(), "MOVIMIENTO", make_pair(5, 0));
                    mov_equipo[k].push_back(movi0);
                    //AGREGAR LOS MOVIMIENTOS DIAGONALES EN ESTOS DE I O J IGUAL A 0
                } else {
                    movi0 = make_tuple(equipoJ[k].p_id(), "MOVIMIENTO", make_pair(8, 0));
                    mov_equipo[k].push_back(movi0);
                    movi0 = make_tuple(equipoJ[k].p_id(), "MOVIMIENTO", make_pair(1, 0));
                    mov_equipo[k].push_back(movi0);
                    movi0 = make_tuple(equipoJ[k].p_id(), "MOVIMIENTO", make_pair(7, 0));
                    mov_equipo[k].push_back(movi0);
                }
            } else if (j == 0) {
                if (i > 0) {
                    movi0 = make_tuple(equipoJ[k].p_id(), "MOVIMIENTO", make_pair(6, 0));
                    mov_equipo[k].push_back(movi0);
                    movi0 = make_tuple(equipoJ[k].p_id(), "MOVIMIENTO", make_pair(7, 0));
                    mov_equipo[k].push_back(movi0);
                    movi0 = make_tuple(equipoJ[k].p_id(), "MOVIMIENTO", make_pair(5, 0));
                    mov_equipo[k].push_back(movi0);
                } else {
                    movi0 = make_tuple(equipoJ[k].p_id(), "MOVIMIENTO", make_pair(2, 0));
                    mov_equipo[k].push_back(movi0);
                    movi0 = make_tuple(equipoJ[k].p_id(), "MOVIMIENTO", make_pair(1, 0));
                    mov_equipo[k].push_back(movi0);
                    movi0 = make_tuple(equipoJ[k].p_id(), "MOVIMIENTO", make_pair(3, 0));
                    mov_equipo[k].push_back(movi0);
                }
            } else {
                if (i > 0) {
                    if (j > 0) {
                        movi0 = make_tuple(equipoJ[k].p_id(), "MOVIMIENTO", make_pair(4, 0));
                        mov_equipo[k].push_back(movi0);
                        movi0 = make_tuple(equipoJ[k].p_id(), "MOVIMIENTO", make_pair(5, 0));
                        mov_equipo[k].push_back(movi0);
                        movi0 = make_tuple(equipoJ[k].p_id(), "MOVIMIENTO", make_pair(6, 0));
                        mov_equipo[k].push_back(movi0);
                    } else {
                        movi0 = make_tuple(equipoJ[k].p_id(), "MOVIMIENTO", make_pair(6, 0));
                        mov_equipo[k].push_back(movi0);
                        movi0 = make_tuple(equipoJ[k].p_id(), "MOVIMIENTO", make_pair(7, 0));
                        mov_equipo[k].push_back(movi0);
                        movi0 = make_tuple(equipoJ[k].p_id(), "MOVIMIENTO", make_pair(8, 0));
                        mov_equipo[k].push_back(movi0);
                    }
                } else {
                    if (j > 0) {
                        movi0 = make_tuple(equipoJ[k].p_id(), "MOVIMIENTO", make_pair(2, 0));
                        mov_equipo[k].push_back(movi0);
                        movi0 = make_tuple(equipoJ[k].p_id(), "MOVIMIENTO", make_pair(3, 0));
                        mov_equipo[k].push_back(movi0);
                        movi0 = make_tuple(equipoJ[k].p_id(), "MOVIMIENTO", make_pair(6, 0));
                        mov_equipo[k].push_back(movi0);
                    } else {
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
        return crearMovValido(t, mov_equipo);
    }

    vector <vector<mov>> generar_mov_ofensivos(LogicalBoard &t) {
        par arc_rival = t.getGoal('A')[1];
        if (nombre == 'A') {
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
            if (nombre == 'A') {
                movi0 = make_tuple(equipoJ[i].p_id(), "MOVIMIENTO", make_pair(3, 0));
                mov_equipo[i].push_back(movi0);
                movi0 = make_tuple(equipoJ[i].p_id(), "MOVIMIENTO", make_pair(4, 0));
                mov_equipo[i].push_back(movi0);
                movi0 = make_tuple(equipoJ[i].p_id(), "MOVIMIENTO", make_pair(5, 0));
                mov_equipo[i].push_back(movi0);
                if (equipoJ[i].tienePelota()) {
                    int f_max = filas / 2;
                    movi0 = make_tuple(equipoJ[i].p_id(), "PASE", make_pair(3, f_max));
                    mov_equipo[i].push_back(movi0);
                    movi0 = make_tuple(equipoJ[i].p_id(), "PASE", make_pair(4, f_max));
                    mov_equipo[i].push_back(movi0);
                    movi0 = make_tuple(equipoJ[i].p_id(), "PASE", make_pair(5, f_max));
                    mov_equipo[i].push_back(movi0);
                }
            } else {
                movi0 = make_tuple(equipoJ[i].p_id(), "MOVIMIENTO", make_pair(1, 0));
                mov_equipo[i].push_back(movi0);
                movi0 = make_tuple(equipoJ[i].p_id(), "MOVIMIENTO", make_pair(7, 0));
                mov_equipo[i].push_back(movi0);
                movi0 = make_tuple(equipoJ[i].p_id(), "MOVIMIENTO", make_pair(8, 0));
                mov_equipo[i].push_back(movi0);
                if (equipoJ[i].tienePelota()) {
                    int f_max = filas / 2;
                    movi0 = make_tuple(equipoJ[i].p_id(), "PASE", make_pair(1, f_max));
                    mov_equipo[i].push_back(movi0);
                    movi0 = make_tuple(equipoJ[i].p_id(), "PASE", make_pair(7, f_max));
                    mov_equipo[i].push_back(movi0);
                    movi0 = make_tuple(equipoJ[i].p_id(), "PASE", make_pair(8, f_max));
                    mov_equipo[i].push_back(movi0);
                }
            }
        }
        return crearMovValido(t, mov_equipo);
    }

    vector <vector<mov>> generar_mov_defensivos(LogicalBoard &t) {
        par riv_ij, diff;
        nombre == 'A' ? riv_ij = t.jugador_con_pelota('B') : riv_ij = t.jugador_con_pelota('A');
        vector<vector<mov>> mov_equipo(3);
        mov movi0;
        vector<Player> equipoJ;
        equipoJ = t.getitem(nombre);
        for (int k = 0; k < 3; ++k) {
            movi0 = make_tuple(equipoJ[k].p_id(), "MOVIMIENTO", make_pair(0, 0));
            mov_equipo[k].push_back(movi0);
            diff = make_pair(riv_ij.first - equipoJ[k].pos_i(), riv_ij.second - equipoJ[k].pos_j());
            if (diff.first == 0) {
                if (diff.second > 0) {
                    movi0 = make_tuple(equipoJ[k].p_id(), "MOVIMIENTO", make_pair(4, 0));
                    mov_equipo[k].push_back(movi0);
                    movi0 = make_tuple(equipoJ[k].p_id(), "MOVIMIENTO", make_pair(3, 0));
                    mov_equipo[k].push_back(movi0);
                    movi0 = make_tuple(equipoJ[k].p_id(), "MOVIMIENTO", make_pair(5, 0));
                    mov_equipo[k].push_back(movi0);
                    //AGREGAR LOS MOVIMIENTOS DIAGONALES EN ESTOS DE I O J IGUAL A 0
                } else {
                    movi0 = make_tuple(equipoJ[k].p_id(), "MOVIMIENTO", make_pair(8, 0));
                    mov_equipo[k].push_back(movi0);
                    movi0 = make_tuple(equipoJ[k].p_id(), "MOVIMIENTO", make_pair(1, 0));
                    mov_equipo[k].push_back(movi0);
                    movi0 = make_tuple(equipoJ[k].p_id(), "MOVIMIENTO", make_pair(7, 0));
                    mov_equipo[k].push_back(movi0);
                }
            } else if (diff.second == 0) {
                if (diff.first > 0) {
                    movi0 = make_tuple(equipoJ[k].p_id(), "MOVIMIENTO", make_pair(6, 0));
                    mov_equipo[k].push_back(movi0);
                    movi0 = make_tuple(equipoJ[k].p_id(), "MOVIMIENTO", make_pair(7, 0));
                    mov_equipo[k].push_back(movi0);
                    movi0 = make_tuple(equipoJ[k].p_id(), "MOVIMIENTO", make_pair(5, 0));
                    mov_equipo[k].push_back(movi0);
                } else {
                    movi0 = make_tuple(equipoJ[k].p_id(), "MOVIMIENTO", make_pair(2, 0));
                    mov_equipo[k].push_back(movi0);
                    movi0 = make_tuple(equipoJ[k].p_id(), "MOVIMIENTO", make_pair(1, 0));
                    mov_equipo[k].push_back(movi0);
                    movi0 = make_tuple(equipoJ[k].p_id(), "MOVIMIENTO", make_pair(3, 0));
                    mov_equipo[k].push_back(movi0);
                }
            } else {
                if (diff.first > 0) {
                    if (diff.second > 0) {
                        movi0 = make_tuple(equipoJ[k].p_id(), "MOVIMIENTO", make_pair(4, 0));
                        mov_equipo[k].push_back(movi0);
                        movi0 = make_tuple(equipoJ[k].p_id(), "MOVIMIENTO", make_pair(5, 0));
                        mov_equipo[k].push_back(movi0);
                        movi0 = make_tuple(equipoJ[k].p_id(), "MOVIMIENTO", make_pair(6, 0));
                        mov_equipo[k].push_back(movi0);
                    } else {
                        movi0 = make_tuple(equipoJ[k].p_id(), "MOVIMIENTO", make_pair(6, 0));
                        mov_equipo[k].push_back(movi0);
                        movi0 = make_tuple(equipoJ[k].p_id(), "MOVIMIENTO", make_pair(7, 0));
                        mov_equipo[k].push_back(movi0);
                        movi0 = make_tuple(equipoJ[k].p_id(), "MOVIMIENTO", make_pair(8, 0));
                        mov_equipo[k].push_back(movi0);
                    }
                } else {
                    if (diff.second > 0) {
                        movi0 = make_tuple(equipoJ[k].p_id(), "MOVIMIENTO", make_pair(2, 0));
                        mov_equipo[k].push_back(movi0);
                        movi0 = make_tuple(equipoJ[k].p_id(), "MOVIMIENTO", make_pair(3, 0));
                        mov_equipo[k].push_back(movi0);
                        movi0 = make_tuple(equipoJ[k].p_id(), "MOVIMIENTO", make_pair(6, 0));
                        mov_equipo[k].push_back(movi0);
                    } else {
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
        return crearMovValido(t, mov_equipo);
    }

    vector <vector<mov>> crearMovValido(LogicalBoard &t, vector <vector<mov>> &ma) {
//        int reserva = 1;
//        for (int i = 0; i < ma.size(); ++i) {
//            reserva *= ma[i].size();
//        }
        vector<Player> nosotros = t.getitem(nombre);
        vector<vector<mov>> v;
        vector<mov> movi;
        for (int i = 0; i < ma[0].size(); ++i) {
            for (int k = 0; k < ma[1].size(); ++k) {
                for (int l = 0; l < ma[2].size(); ++l) {
                    movi.push_back(ma[0][i]);
                    movi.push_back(ma[1][k]);
                    movi.push_back(ma[2][l]);
                    if (t.isValidTeamMove(nosotros, movi)) v.push_back(movi);
                    movi.clear();
                }
            }
        }
        return v;
    }

    vector <mov> elegirMov(LogicalBoard &t, vector <vector<mov>> &v) {
        char nom;
        bool p1 = true;
        nombre == 'A' ? nom = 'B' : nom = 'A';
        auto rivales = t.getitem(nom);
        mov m0;
        vector<mov> parado;
        for (int j = 0; j < 3; ++j) {
            m0 = make_tuple(rivales[j].p_id(), "MOVIMIENTO", make_pair(0, 0));
            parado.push_back(m0);
        }

        pair<float, int> max;
        float tmp;
        if (nom == 'A') {
            for (int i = 0; i < v.size(); ++i) {
                t.makeMove(parado, v[i]);
                tmp = puntuarTablero(t);
                if (i == 0 || tmp > max.first) {
                    max.first = tmp;
                    max.second = i;
                }
                t.undoMove();
            }
        }
        else {
            int i = 0;
            for (i; i < v.size(); ++i) {
                t.makeMove(v[i], parado);
                tmp = puntuarTablero(t);
                //cout << tmp << endl;
                if (i == 0 || tmp > max.first) {
                    max.first = tmp;
                    max.second = i;
                }
                t.undoMove();
                p1 = (t.getitem('A')[2].pos_i() == 2) && (t.getitem('A')[2].pos_j() == 5);
            }
        }
        return v[max.second];
    }

    float puntuarTablero(LogicalBoard &t) {
        if (t.pelota_libre()) {
            return puntuar_libre(t);
        } else if (t.posesion(nombre)) {
            return puntuar_ofensiva(t);
        } else {
            return puntuar_defensiva(t);
        }
    }

    vector<float> &damePesos() {
        return pesos;
    }

    int &dameTurnos() {
        return turnos;
    }

    void cambiarNombre(char nom){
        nombre = nom;
    }

    int dameFilas(){return filas;}

    int dameColumnas(){return columnas;}

private:
    //podria cambiar la distancia de cada uno
    int filas;
    int columnas;
    int turnos;
    char nombre;
    bool izq;
    // de la posicion 0 a 2 estan los p.quite de cada jugador
    // en la posicion 3 esta la distancia al arco tiene que ser negativo (cuanto mas lejos peor)
    // en la posicion 4 es el peso del angulo del tiro del jugador con la pelota.
    // en la posicion 5 es el peso de ser goleado (tiene que ser negativo)
    // en la posicion 6 es el peso de hacer un gol
    // en la posicion 7 esta la distancia al rival con pelota NEGATIVO
    // en la posicion 8 esta la distancia a la pelota libre, (negativo).
    // en la posicion 9 la pelota esta yendo al arco
    // en la posicion 10 hay un rival en la trayectoria de la pelota

    //quizas estaria bueno tener un peso mas y separar la funcion que devuelve si un rival o yo estoy en
    //trayectoria
    vector<float> pesos;
};

void imprimirJugadas(LogicalBoard &t, int i){
    vector<Player> team1 = t.getitem('A');
    vector<Player> team2 = t.getitem('B');

    cout << "-----------------------------------------Jugada "<< i <<"-----------------------------------" << endl;

    cout << "-------------------------------------------Team A------------------------------------" << endl;
    for (int i = 0; i < 3; ++i) {
        team1[i].imprimirJugador();
    }
    cout << "-------------------------------------------Team B------------------------------------" << endl;
    for (int i = 0; i < 3; ++i) {
        team2[i].imprimirJugador();
    }

    if(t.pelota_libre()){
        cout << "--------------------------------------Pelota Sin Posesion------------------------------------" << endl;
        t.dame_pelota_libre().imprimirPelota();
    }
}

void imprimirPesos(vector<float> &weights){
    cout << "[ ";
    for (auto i = 0; i < weights.size(); i++){
        i != weights.size()-1? cout << weights[i] << ", ":cout << weights[i] << "]";
    }
    cout << endl;
}

par jugar(Team &a, Team &b, LogicalBoard &t) {
    vector<mov> teamplay_a;
    vector<mov> teamplay_b;

    int match_duration = a.dameTurnos();

    //imprimirJugadas(t, 0);

    int i = 0;
    for (i; i < match_duration; ++i) {
        teamplay_a = a.generarJugada(t);
        teamplay_b = b.generarJugada(t);
//        if (i == 0){
//            teamplay_a = {make_tuple(0, "PASE", make_pair(4, 1)),
//                          make_tuple(1, "MOVIMIENTO", make_pair(0, 0)),
//                          make_tuple(2, "MOVIMIENTO", make_pair(0, 0))};
//            teamplay_b = {make_tuple(3, "MOVIMIENTO", make_pair(0, 0)),
//                          make_tuple(4, "MOVIMIENTO", make_pair(0, 0)),
//                          make_tuple(5, "MOVIMIENTO", make_pair(0, 0))};
//        }
//        else if(i == 1){
//            teamplay_a = {make_tuple(0, "MOVIMIENTO", make_pair(2, 0)),
//                          make_tuple(1, "MOVIMIENTO", make_pair(0, 0)),
//                          make_tuple(2, "MOVIMIENTO", make_pair(0, 0))};
//
//            teamplay_b = {make_tuple(3, "MOVIMIENTO", make_pair(8, 0)),
//                          make_tuple(4, "MOVIMIENTO", make_pair(0, 0)),
//                          make_tuple(5, "MOVIMIENTO", make_pair(0, 0))};
//        }
//        else if(i < 5){
//            teamplay_a = {make_tuple(0, "MOVIMIENTO", make_pair(0, 0)),
//                          make_tuple(1, "MOVIMIENTO", make_pair(0, 0)),
//                          make_tuple(2, "MOVIMIENTO", make_pair(0, 0))};
//
//            teamplay_b = {make_tuple(3, "MOVIMIENTO", make_pair(8, 0)),
//                          make_tuple(4, "MOVIMIENTO", make_pair(0, 0)),
//                          make_tuple(5, "MOVIMIENTO", make_pair(0, 0))};
//        }
//        else if (i == 5){
//            teamplay_a = {make_tuple(0, "MOVIMIENTO", make_pair(0, 0)),
//                          make_tuple(1, "MOVIMIENTO", make_pair(0, 0)),
//                          make_tuple(2, "MOVIMIENTO", make_pair(0, 0))};
//
//            teamplay_b = {make_tuple(3, "PASE", make_pair(8, 2)),
//                          make_tuple(4, "MOVIMIENTO", make_pair(0, 0)),
//                          make_tuple(5, "MOVIMIENTO", make_pair(0, 0))};
//        }
//        else{
//            teamplay_a = {make_tuple(0, "MOVIMIENTO", make_pair(0, 0)),
//                          make_tuple(1, "MOVIMIENTO", make_pair(0, 0)),
//                          make_tuple(2, "MOVIMIENTO", make_pair(0, 0))};
//
//            teamplay_b = {make_tuple(3, "MOVIMIENTO", make_pair(0, 0)),
//                          make_tuple(4, "MOVIMIENTO", make_pair(0, 0)),
//                          make_tuple(5, "MOVIMIENTO", make_pair(0, 0))};
//        }


        t.makeMove(teamplay_a, teamplay_b);

        //imprimirJugadas(t, i+1);
    }

    return t.resultado();
}

vector<float> campOff(Team &original, LogicalBoard &t, vector<par> &posA, vector<par> &posB){
    vector<vector<float>> pv(4);
    vector<float> single;
    float k;
    int contando = 1;
    int indice = 0;
    for (int i = 0; i < 6; ++i) {
        k = original.damePesos()[i];
        pv[indice].push_back(k);
        if (original.damePesos()[i] < 0.97) {
            k = k + 0.04;
            pv[indice].push_back(k);
        }
        if (original.damePesos()[i] > -0.97) {
            k = original.damePesos()[i] - 0.04;
            pv[indice].push_back(k);
        }
        if(i == 0) i = 2;
        indice++;
    }

    int cantGanadas = 0;
    bool prim_it;
    for (int j = 0; j < pv[0].size(); ++j) {
        for (int m = 0; m < pv[1].size() ; ++m) {
            for (int n = 0; n < pv[2].size(); ++n) {
                for (int i1 = 0; i1 < pv[3].size(); ++i1) {
                    prim_it = j == 0 && m == 0 && n == 0 && i1 == 0;
                    if(!prim_it){
                        single.push_back(pv[0][j]);
                        single.push_back(original.damePesos()[1]);
                        single.push_back(original.damePesos()[2]);
                        single.push_back(pv[1][m]);
                        single.push_back(pv[2][n]);
                        single.push_back(pv[3][i1]);
                        for (int k1 = 6; k1 < 11; ++k1) {
                            single.push_back(original.damePesos()[k1]);
                        }
                        t.cambiarPesos({single[0], single[1], single[2]}, 'B');
                        t.reset(posA, posB);
                        Team b(original.dameFilas(), original.dameColumnas(), 'B', single, original.dameTurnos());
                        for (int l1 = 0; l1 < 20; ++l1) {
                            jugar(original, b, t);
                            if (t.winner() == 'B') cantGanadas++;
                            t.reset(posA, posB);
                            //cout << "iteracion numero: " << l1 << endl;
                            //cout << "el res es: " << r.first << ", " << r.second << endl;
                        }
                        cout << "termino el partido: " << contando << endl;
                        cout << "El equipo B gano: " << cantGanadas << endl;
                        if(cantGanadas >= 15) return single;
                        cantGanadas = 0;
                        single.clear();
                        contando++;
                    }
                }
            }
        }
    }
    return original.damePesos();
}

vector<float> campDeff(Team &original, LogicalBoard &t, vector<par> &posA, vector<par> &posB){
    //6, 7, 1, 2
    vector<vector<float>> pv(4);
    vector<float> single;
    float k;
    int contando = 1;
    int indice = 0;
    for (int i = 1; i < 8; ++i) {
        k = original.damePesos()[i];
        pv[indice].push_back(k);
        if (original.damePesos()[i] < 0.97) {
            k = k + 0.04;
            pv[indice].push_back(k);
        }
        if (original.damePesos()[i] > -0.97) {
            k = original.damePesos()[i] - 0.04;
            pv[indice].push_back(k);
        }
        if (i == 2) i = 5;
        indice++;
    }

    int cantGanadas = 0;
    bool prim_it;
    for (int j = 0; j < pv[0].size(); ++j) {
        for (int m = 0; m < pv[1].size() ; ++m) {
            for (int n = 0; n < pv[2].size(); ++n) {
                for (int i1 = 0; i1 < pv[3].size(); ++i1) {
                    prim_it = j == 0 && m == 0 && n == 0 && i1 == 0;
                    if(!prim_it){
                        single.push_back(original.damePesos()[0]);
                        single.push_back(pv[0][j]);
                        single.push_back(pv[1][m]);
                        for (int k1 = 3; k1 < 6; ++k1) single.push_back(original.damePesos()[k1]);
                        single.push_back(pv[2][n]);
                        single.push_back(pv[3][i1]);
                        for (int k1 = 8; k1 < 11; ++k1) single.push_back(original.damePesos()[k1]);
                        t.cambiarPesos({single[0], single[1], single[2]}, 'B');
                        t.reset(posA, posB);
                        Team b(original.dameFilas(), original.dameColumnas(), 'B', single, original.dameTurnos());
                        for (int l1 = 0; l1 < 20; ++l1) {
                            jugar(original, b, t);
                            if (t.winner() == 'B') cantGanadas++;
                            t.reset(posA, posB);
                            //cout << "iteracion numero: " << l1 << endl;
                            //cout << "el res es: " << r.first << ", " << r.second << endl;
                        }
                        cout << "termino el partido: " << contando << endl;
                        cout << "El equipo B gano: " << cantGanadas << endl;
                        if(cantGanadas >= 15) return single;
                        cantGanadas = 0;
                        single.clear();
                        contando++;
                    }
                }
            }
        }
    }
    return original.damePesos();
}

Team compLocal(Team &inicial, LogicalBoard &t, vector<par> &posA, vector<par> &posB) {
    imprimirPesos(inicial.damePesos());
    for (int i = 0; i < 200; ++i) {
        inicial.damePesos() = campOff(inicial, t, posA, posB);
        vector<float> current = {inicial.damePesos()[0], inicial.damePesos()[1], inicial.damePesos()[2]};
        t.cambiarPesos(current, 'B');
        imprimirPesos(inicial.damePesos());
        inicial.damePesos() = campDeff(inicial, t, posA, posB);
        current = {inicial.damePesos()[0], inicial.damePesos()[1], inicial.damePesos()[2]};
        t.cambiarPesos(current, 'B');
        imprimirPesos(inicial.damePesos());
    }
    return inicial;
    ///aca van a jugar
    //aca me gano uno
}

vector<vector<float> > populacion() {
    vector<vector<float>> poblacion(12);
    for (int i = 0; i < 12; ++i) {   //i es la cantidad de poblaciones distintas que se generan que van a ser 12 en total
        for (int j = 0; j < 11; ++j) { //j son los indices de los pesos que van de 0 a 10
            if (j == 0 || j == 1 || j == 2 || j == 4 || j == 6 || j == 9 || j == 10) {
                poblacion[i].push_back(rand() / (float) RAND_MAX);  //genera numeros random en el intervalo [0..1]
            } else {
                poblacion[i].push_back(-rand() / (float) RAND_MAX); //genera numeros random en el intervalo [-1..0]
            }
        }
    }
    return poblacion;
}

vector<float> fitnessUno(vector<vector<float> >& poblacion, int& turnos, LogicalBoard& t){
    vector<Team> equipos;
    vector<float> puntaje(12, 0);
    for (int i = 0; i < 12; ++i) { //En principio todos los equipos se llaman B, luego cambio a A aquel que va a competir con todos los demas
        equipos.push_back(Team(t.filas(), t.columnas(), 'B', poblacion[i], turnos));
    }
    for (int i = 0; i < 12; ++i) {
        for (int j = i+1; j <12 ; ++j) {
            equipos[i].cambiarNombre('A');
            for (int k = 0; k < 19; ++k) {   //Hago jugar a cada par de equipos 20 partidos
                jugar(equipos[i], equipos[j], t);
                if(t.winner() == 'A'){  //gano le sumo 3
                    puntaje[i] +=3;
                }else if(t.winner() == 'E'){//empato le sumo 1 y sino no le sumo nada
                    puntaje[i] += 1;
                    puntaje[j] += 1;
                }else{
                    puntaje[j] += 3;
                }
            }
        }

    }
    return puntaje;
}

//fitnessDos suma puntos en funcion de partidos ganados o empatados + la diferencia de goles en caso de ganar
vector<float> fitnessDos(vector<vector<float>>& poblacion, int& turnos, LogicalBoard& t){
    int dif_goles;
    par marcador;
    vector<Team> equipos;
    vector<float> puntaje(12, 0);
    for (int i = 0; i < 12; ++i) { //En principio todos los equipos se llaman B, luego cambio a A aquel que va a competir con todos los demas
        equipos.push_back(Team(t.filas(), t.columnas(), 'B', poblacion[i], turnos));
    }
    for (int i = 0; i < 12; ++i) {
        for (int j = i+1; j <12 ; ++j) {
            equipos[i].cambiarNombre('A');
            for (int k = 0; k < 19; ++k) {   //Hago jugar a cada par de equipos 20 partidos
                jugar(equipos[i], equipos[j], t);
                if(t.winner() == 'A'){  //gano le sumo 3
                    marcador = t.resultado();
                    dif_goles = marcador.first - marcador.second;
                    puntaje[i] += 3 + dif_goles;
                }else if(t.winner() == 'E'){//empato le sumo 1 y sino no le sumo nada
                    puntaje[i] += 1;
                    puntaje[j] += 1;
                }else{
                    marcador = t.resultado();
                    dif_goles = marcador.second - marcador.first;
                    puntaje[j] += 3 + dif_goles;
                }
                dif_goles = 0;
            }
        }

    }
    return puntaje;
}
//agarro la primer mitad de uno y lo pongo con la segunda mitad del otro
void crossoverUno(vector<float>& p1, vector<float>& p2){
    float finDelDos;
    for (int i = p1.size()/2; i < p1.size(); ++i) {
        finDelDos = p2[i];
        p2[i] = p1[i];
        p1[i] = finDelDos;
    }
}

//Mezclo de a cuartos primer cuarto queda igual segundo cuarto se intercambia tercer cuarto queda igual cuarta parte se intercambia
void crossoverDos(vector<float>& p1, vector<float>& p2) {
    float segundoCuarto, ultimoCuarto;
    for (int i = p1.size()/4; i < p1.size()/2; ++i) {
        segundoCuarto = p2[i];
        p2[i] = p1[i];
        p1[i] = segundoCuarto;
    }
    for (int i = 3*p1.size()/4; i < p1.size(); ++i) {
        ultimoCuarto = p2[i];
        p2[i] = p1[i];
        p1[i] = ultimoCuarto;
    }
}

vector<float> mutacion(vector<float> p){
    for (int i = 0; i <p.size() ; ++i) {
        if(rand()/(float) RAND_MAX < 0.05){
            if((i == 0 || i == 1 || i == 2 || i == 4 || i == 6 || i == 9 || i == 10) && p[i] < 0.96){
                p[i] += 0.05;
            }else if((i == 3 || i == 5 || i == 7 || i == 8) && p[i] > -0.96){
                p[i] -= 0.05;
            }
        }
    }
    return p;
}
//Elijo la mitad de la poblacion de manera random
vector<vector<float> > seleccionSimple(vector<vector<float> > poblacion){
    vector<vector<float>> seleccion;
    int indice;
    for (int i = 0; i < 5; ++i) {
        indice = rand() % 6;
        seleccion.push_back(poblacion[indice]);
    }
    return seleccion;
}

vector<vector<float> > seleccionPonderada(vector<vector<float> > poblacion,int& turnos, LogicalBoard& t) {
    vector<float> fitness = fitnessUno(poblacion, turnos, t);
    float sumaFitness = suma(fitness);
    vector<vector<float> > seleccionados;
    vector<float> probabilidades;
    int j = 0;
    for (int i = 0; i < poblacion.size(); ++i) {
        probabilidades.push_back(0.5 * fitness[i] / sumaFitness);  //normalizo los valores de fitness
    }
    while (seleccionados.size() < 6){
        while (j < probabilidades.size()) {
            if (rand() / (float) RAND_MAX <= probabilidades[j]) {
                seleccionados.push_back(poblacion[j]);
                probabilidades[j] = 0;   //si ya salio elegido me aseguro que no vuelva a salir elegido.
            }
            ++j;
        }
        j = 0;
    }
    return seleccionados;
}

int main() {
    srand(time(NULL));

    vector<float> weights;

    for (auto i = 0; i < 11; i++){
        if (i == 0 || i == 1 || i == 2 || i == 4 || i == 6 || i == 9 || i == 10){
            weights.push_back(rand()/ (float)RAND_MAX);
        } else {
            weights.push_back(-rand()/ (float)RAND_MAX);
        }
    }

    float quite = 0.5;
    //weights = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

    vector<pair<int, float>> team_1 = {make_pair(0, weights[0]), make_pair(1, weights[1]), make_pair(2, weights[2])};
    vector<pair<int, float>> team_2 = {make_pair(3, quite), make_pair(4, quite), make_pair(5, quite)};
    LogicalBoard tablero(10, 5, team_1, team_2);

    vector<par> posA = {make_pair(2, 3), make_pair(1, 2), make_pair(3, 2)};
    vector<par> posB = {make_pair(2, 6), make_pair(1, 7), make_pair(3, 7)};
//    tablero.reset(posA, posB);
//
//    auto test = tablero.pelota_libre();
//

//    weights.push_back(quite); // pesos[0]
//    weights.push_back(quite); // pesos[1]
//    weights.push_back(quite); // pesos[2]
//
//    weights.push_back(-0.62); // pesos[3] distancia al arco rival
//    weights.push_back(0.65); // pesos[4] angulo de tiro.
//    weights.push_back(-0.70); // pesos[5] me metieron un gol.
//    weights.push_back(0.90); // pesos[6] hice un gol.
//    weights.push_back(-0.57); // pesos[7] distancia al rival con pelota
//    weights.push_back(-0.83); // pesos[8] distancia a la pelota libre
//    weights.push_back(0.70); // pesos[9] la pelota yendo al arco
//    weights.push_back(0.71); // pesos[10] hay un rival en la trayectoria de la pelota.

    Team a(5, 10, 'A', weights, 20);
//    Team b(5, 10, 'B', weights, 20);

//    vector<mov> jugada_A = {make_tuple(0, "PASE", make_pair(4, 2)),
//                            make_tuple(1, "MOVIMIENTO", make_pair(0, 0)),
//                            make_tuple(2, "MOVIMIENTO", make_pair(0, 0))};
//
//    vector<mov> jugada_B = {make_tuple(3, "MOVIMIENTO", make_pair(0, 0)),
//                            make_tuple(4, "MOVIMIENTO", make_pair(0, 0)),
//                            make_tuple(5, "MOVIMIENTO", make_pair(0, 0))};

//    tablero.makeMove(jugada_A, jugada_B);

    compLocal(a, tablero, posA, posB);

//    imprimirPesos(a.damePesos());

//    par resultado = jugar(a, b, tablero);

//    cout << "Goles de TEAM A: " << resultado.first << endl;
//    cout << "Goles de TEAM B: " << resultado.second << endl;

//    par m = make_pair(0, 0);
//    vector<mov> jugada_B = {make_tuple(3, "MOVIMIENTO", m), make_tuple(4, "MOVIMIENTO", m), make_tuple(5, "MOVIMIENTO", m)};
//    par m1 = make_pair(1, 1);
//    par m2 = make_pair(3, 0);
//    par m3 = make_pair(4, 0);
//    auto team1 = tablero.getitem('A');
//    auto team2 = tablero.getitem('B');
//
//    for (int i = 0; i < 3; ++i) {
//        team1[i].imprimirJugador();
//    }
//    for (int i = 0; i < 3; ++i) {
//        team2[i].imprimirJugador();
//
//    }
//    if(tablero.pelota_libre()){
//        tablero.dame_pelota_libre().imprimirPelota();
//    }



//    populacion();



    return 0;
}
