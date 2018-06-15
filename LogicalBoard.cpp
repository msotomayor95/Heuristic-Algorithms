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
Trabajo Practico III

typedef pair<int,int> par;
typedef vector<par> movimientos;


par pos0 = make_pair(0,0);
par pos1 = make_pair(-1,-1);
par pos2 = make_pair(-1,0);
par pos3 = make_pair(-1,1);
par pos4 = make_pair(0,1);
par pos5 = make_pair(1,1);
par pos6 = make_pair(1,0);
par pos7 = make_pair(1,-1);
par pos8 = make_pair(0,-1);


movimientos moves ={pos0, pos1, pos2, pos3, pos4, pos5, pos6, pos7, pos8};

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
	
	 void imprimirPosicion(){
        
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
	
	void imprimirPosicion(){
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

LogicalBoard(int columns, int rows, vector<par> team_1, vector<par> team_2):{
assert(rows mod 2 == 1 && rows>=3);
assert(columns mod 2 == 0 && columns>=2*rows);
par inicio = make_pair(0, 0);
score = inicio;
for(int i=0; i<3; i++){ //team_1 contiene pares (p_id, p_quite) de los 3 jugadore del equipo A
team_A.push_back(Player(team_1[i].first, team_1[i].second))  


}

team_A = team_1;
team_B = team_2;

}

private:
par score;
vector<Player> team_A;   
vector<Player> team_B;   	
int columns;
int rows;
vector<int> goal_rows;
vector<par> goal_A;
vector<par> goal_B;
//free_ball;  falta definir el tipo
//last_state;  falta definir el tipo





};



int main(){

	
	

return 0;	
} 
