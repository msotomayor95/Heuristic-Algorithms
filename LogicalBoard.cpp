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

Trabajo Practico III

typedef pair<int,int> par;
typedef vector<par> movimientos;
typedef tuple<int,int,int,string> posicion;

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


class Player{
public:

	Player(int player_id, double pQuite=0.5, bool tienePelota=false): id(player_id), p_quite(pQuite), hayPosesion(tienePelota) {}  //constructor de jugador
	
	void move(int move){
		old_position = make_pair(jug_i, jug_j);
		jug_i += moves[move].first; 
		jug_j += moves[move].second;
		moveBall();
	
	}
	
	void backwardMove(int move){
		old_position = make_pair(jug_i, jug_j);	
		jug_i -= moves[move].first; 
		jug_j -= moves[move].second;
		moveBall();
	}
	
	void undoMove(){
        par p = make_pair(jug_i,jug_j);
		if(p != old_position){
			jug_i = old_position.first;
			jug_j = old_position.second;
			moveBall();
		}else{
			//se rompe
			
		}
        // solo guarda una jugada, 
        // si quieren deshacer mas, se rompe a proposito
        //old_position = None; //Ver como representar esto en c++, quizas no es necesario porque a menos que haga un nuevo movimiento old_position sigue valiendo lo mismo
		
	}
	
	void moveBall(){   //el jugador se mueve con la pelota y por lo tanto la pelota tiene las mismas coordenadas que el jugador
		if(hayPosesion){						// if not self.ball is None:   -------ver esto!-----Creo que seria definir que el jugador tiene que tener la pelota
			ball.pel_i = jug_i;
			ball.pel_j = jug_j;
		}
	}
	
	void takeBall(Ball pelota){
		hayPosesion = true;
		ball = pelota;	
		//ball.setMovement(None);  //ver el None
		moveBall();
	}
	
	void imprimirPosicion(){
		posicion position; 
		position = make_tuple(id, jug_i, jug_j, "IN_POSETION");	
		if(!hayPosesion){
			position =  make_tuple(id, jug_i, jug_j, "FREE_PLAYER");  			
		}
		cout<< get<0>(position) << " " << get<1>(position) << " " << get<2>(position) << " " << get<3>(position) << endl;
		
	}

	/*	def __str__(self):
        position = (self.id, self.i, self.j, IN_POSETION)
        if self.ball is not None:   ---------------Ver esto no se entiende bien, no seria al reves? Si la pelota no es None esta definida y por lo tanto el jugador la posee
            position = (self.id, self.i, self.j, FREE_PLAYER)

        return str(position)
*/
	 
	
	
private:
	
	int id;
	double p_quite;
	par old_position;
    int jug_i;
    int jug_j;    
    Ball ball;
    bool hayPosesion;   //agrego este bool para la funcion moveBall 
	
	
}; 

int main(){

	
	

return 0;	
} 
