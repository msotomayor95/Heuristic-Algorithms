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
#include <stdlib.h>
 
Trabajo Practico III

typedef pair<int,int> par;
typedef vector<par> movimientos;
typedef tuple<int, string, par> mov;

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

bool pertenece(int e, vector<int> v){
	bool res = false;
	for(uint i = 0; i<v.size(); i++){
		if (e == v[i]){
			res = true;
			return res;	
		}	
	}
	return res;	
}  

class Ball{
	public:
	//Como no le asigna ningun valor a los parametros, no se necesita definir el constructor, c++ llama al constructor por defecto 
	
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
	
	vector<par> trajectory(){   //Me da el vector conteniendo la trayectoria de la pelota para un movimiento dado
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
	
	
	private:
	par movement;	
	bool tieneMov = false; //lo agrego para la funcion move  
	int pel_i;
	int pel_j;
}; 

class Player{
public:

	Player(int player_id, double pQuite=0.5, bool tienePelota=false): id(player_id), p_quite(pQuite), hayPosesion(tienePelota) {}  //constructor de jugador
	
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
		if(ball.tieneMov()){
			ball.movement.second = 0;   //Le anulo la cantidad de pasos restantes es como si hubiera llegado a destino	
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

private:
	
	int id;
	double p_quite;
	par old_position;
    int jug_i;
    int jug_j;    
    Ball ball;
    bool hayPosesion;   //agrego este bool para la funcion moveBall 
	
	
};

class LogicalBoard{
public:

LogicalBoard(int columnas, int filas, vector<par> team_1, vector<par> team_2):{   //Hay que ver bien cual es el tipo de team_1 y team_2 puese ser que no sean pares 
	assert(rows mod 2 == 1 && rows>=3);
	assert(columns mod 2 == 0 && columns>=2*rows);
	par inicio = make_pair(0, 0);
	score = inicio;
	columns = columnas;
	rows = filas;
	par A,B;
	for(int i=0; i<3; i++){ //asumo que team_1 contiene pares (p_id, p_quite) de los 3 jugadores del equipo A
		team_A.push_back(Player(team_1[i].first, team_1[i].second));
		team_B.push_back(Player(team_2[i].first, team_2[i].second));  
		goal_rows.push_back.((rows div 2) - 1 + i);	
		A = make_pair(goal_rows[i],-1);
		B = make_pair(goal_rows[i]; columns);
		goal_A.push_back(A);
		goal_B.push_back(B); 
	}
}
// moves = [(player_id, move_type, value)] 
// value en [0, ..., 8] o (dir, steps) con dir en [1, ..., 8] y step en [0, ..., inf]

    bool isValidTeamMove(vector<Player> team, /*ver tipo*/vector<mov> moves):
        valid = true;
		vector<int> player_ids;
        # Exactamente un movimiento por jugador del equipo  //o sea len(moves) == 3
        for(int i=0; i<team.size(); i++){
			 player_ids.push_back(team[i].id);
		}
		vector<int> idEnMoves;
		for(uint i=0; i<moves.size(); i++){
			if(pertenece(get<0>(moves[i]), player_ids){
				idEnMoves.push_back(get<0>(moves[i])); 
			}
		}
		valid = valid && (moves.size() == team.size());
		valid = valid && (moves.size() == idEnMoves.size()); //verifique asi que los movimientos eran los de los jugadores del equipo y no hay ningun otro jugador en moves 
       

        # muevo los jugadores
        for p in team:
            player_move = moves[p.id]
            if player_move['move_type'] == MOVIMIENTO: 
                p.move(player_move['value'])
            elif p.ball is None:
                valid = False # Quiere pasar la pelota pero no la tiene
            else:
                # Mirar que el pase es válido: O sea que termina adentro de la cancha, en algún 
                # arco o cruza un arco (ya que va de a dos pasos por vez).
                # Además, no puede ser más largo que M / 2
                valid = valid and player_move['value'][1] <= self.rows / 2

                Ball ball;  //crea una pelota por defecto
                ball.i = p.i
                ball.j = p.j
                cout << "is valid move ball" << endl;
                //print ball
                ball.imprimirPelota();
                ball.setMovement(player_move['value'])  //falta esta linea
                //print ball
                ball.imprimirPelota();
                vector<par> ball_trajectory = ball.trajectory();
                print ball_trajectory
                for(uint i=0; i<ball_trajectory.size(); i++){
					cout << "(" << ball_trajectory[i].first << ", " << ball_trajectory[i].second << ") ";	
				}
				cout << endl;
                cout << "is valid move ball" << endl;
                trajectory_in_board = all([self.positionInBoard(t[0], t[1]) for t in ball_trajectory])
                trajectory_in_goal = any([t in self.goal_A + self.goal_B for t in ball_trajectory])
                valid = valid and (trajectory_in_board or trajectory_in_goal)

        # Dos jugadores del mismo equipo estan en la misma posicion
        valid = valid and len(set([(p.i, p.j) for p in team])) == len(team)

        # Todos los jugadores deben estar dentro de la cancha
        for p in team:
            # Tambien puede estar en un arco si es un jugador con pelota
            if p.ball is not None and (p.i, p.j) in self.goal_A + self.goal_B:
                continue
            valid = valid and self.positionInBoard(p.i, p.j)

        # Deshago los movimientos
        for p in team:
            if moves[p.id]['move_type'] == MOVIMIENTO: 
                p.undoMove()

        return valid

private:
par score;   //puntaje del partido
vector<Player> team_A;   
vector<Player> team_B;   	
int columns;
int rows;
vector<int> goal_rows; //filas del arco 
vector<par> goal_A;   //coordenadas del arco
vector<par> goal_B;
//free_ball;  falta definir el tipo
//last_state;  falta definir el tipo





};



int main(){

	
	

return 0;	
} 
