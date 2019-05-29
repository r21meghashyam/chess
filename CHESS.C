#include<graphics.h>
#include<math.h>
#include<string.h>
#include<time.h>


/*
 *Code Repository: http://github.com/r21meghashyam/CG-Chess
 *Key Concepts:
 *	pawns:
		S is soilder
		E is elephant
		H is knight (H as Horse since K is for king)
		B is for Bishop
		Q is Queen
		K is king
		N to repesent that the cell is not holding any pawn
	pr and pc is tracing the location of the red cell which happens
	to be the keyboard pointer
	player[2]:
		  player[0]: at the top
		  player[1]: at the bottom
	Game is designed giving importance to the cells rather than pawn

	In comments, term 'map' or 'mapping' means to set a cell value with
	pawn type and color

	token 'map' which is an array with 8x8=64 elements of the structure
	'cell'is a blue print of all the cells, pawns in cells and the color
	of both pawn and cell

	Render: to draw or redraw a small portion or the entier screen.

	"A GOOD PROGRAMER READS THE PROGRAM LIKE THE COMPILER DOES"
	"BUT DOESNT GET STUCK ON LOOPS"
 */

void select();//pre-declared for some reasons
int	startx,
	midx,
	endx,
	starty,
	midy,
	endy,
	cellsize,
	pc,
	pr,
	turn,
	cpr,
	cpc,
	pawnup,
	player[2],
	paths[21];

typedef struct {
	char pawn;
	int pawncolor;
	int cellcolor;
	}cell;

//to keep record of all the 64 cells in the board
cell map[8][8];


//retun the row location (y value)
int row(int value){
   return starty + value*cellsize;
}

//retun the coloum location (x value)
int col(int value){
	return startx + value*cellsize;
}

//return the mid point the the cell
//can be used for both x and y
int center(int value){
	return value + cellsize/2;
}

//add a pawn to a particular cell
void setcell(int r,int c,char p,int col){
	/*
	r: row of the cell
	c: column of the cell
	p: pawn to be added
	col: color of the pawn
	*/
	map[r][c].pawn=p;//set the pawn
	map[r][c].pawncolor=col;//set the pawn color
}

//remove pawn and reset to default color
void clearcell(int r,int c){
	/*
	r: row of the cell
	c: column of the cell
	*/
	int color;
	color = r%2==c%2?WHITE:BLACK;
	/*
	r%2 and c%2 will give 1 for odd row
		      0 for even row
	ex: 3%2 => 1
	    6%2 => 0
	if both the r%2 and c%2 gives the same output then we set cell
	color as white else black
	*/
	setfillstyle(SOLID_FILL,color);
	/*
	set the fill color before calling bar
	*/
	bar(col(c),row(r),col(c)+cellsize,row(r)+cellsize);
	/*
	filling the cell with the cell color
	bar(x-start,y-start,x-end,y-end)
	col(c): get x value of 'c'th column
	row(r): get y value of 'r'th row
	+cellsize: so that it fills the whole cell
	*/
	map[r][c].pawn='N';
	//since no pawn will be there after clearing set the
	//pawn to N which means Nothing
	map[r][c].pawncolor=-1;
	//set pawn color to -1 since no pawn exists
	map[r][c].cellcolor=color;//set the selected color

}

//initalize graph, set end points, set player sides, map pawns
void init(){
	int	graphdevice,
		graphmode,
		side,
		i,j,
		rowno;
	graphdevice = DETECT;//Detect screen size
	pawnup=0;
	initgraph(&graphdevice,&graphmode,"C:/TC/BGI");
	/*
	Load and initalize graphics
	*/

	endx = midx = getmaxx()+1;
	/*
	set the endx and midx to  get the screen width size
	commonly returns 639, add 1 to make 640
	*/
	endy = midy = getmaxy()+1;
	/*
	set the endy and midy to get the screen height size
	commonly returns 279, add 1 to make 480
	*/

	if(midx>midy){ //if screen width is bigger than height
		startx = (midx-midy)/2;
		//set startx
		/*let 	midx=640
			midy=480
			startx	=(540-480)/2
				= 160/2
				=80
		*/
		endx = startx+midy;
		//set endx
		/*let 	startx=80
			midy=480
			endx	=80+480
				=560
		*/
		cellsize = midy/8;
		/*let	midy=480
			cellsize =480/8
				 =80
		*/
	}
	else{   //This might not happen
		starty = (midy-midx)/2;
		//set starty
		/*let 	midy=640
			midx=480
			starty	=(640-480)/2
				= 160/2
				=80
		*/
		endy = starty+midx;
		//set endy
		/*let 	starty=80
			midx=480
			endy	=80+480
				=560
		*/
		cellsize = midx/9;
		/*let	midx=480
			cellsize =480/8
				 =80
		*/
	}

	player[0]=BLACK;
	/*
	Made player[0](top one) White by default
	*/
	player[1]=player[0]==WHITE?BLACK:WHITE;
	/*
	setting the oppotsite color of player[0] to player[1]
	player[0]==WHITE: if true, return black else white
	*/
	turn = WHITE;
	//White plays the first move
	//mapping all the 64 cells with pawn as N for nothing and color as -1
	for(i=0;i<8;i++)//for rows
		for(j=0;j<8;j++){//for cols
			map[i][j].pawn='N';
			map[i][j].pawncolor=-1;

		}
	//Set the pawns for both sides together

	for(side=0;side<2;side++){//loops two times, once for player[0], then for player[1]
		/*
			mapping the soliders
			solider will be placed in row 1 and 6 for all the columns
			row 1 for player[0]
			row 6 for player[1]
		*/

		for(i=0;i<8;i++){//lopping from col 0 to 7
			rowno= side==0?1:6;
			/*
				if the side is 0, ie player[0]
				rowno is 1 to indicate top soilders
				if the side is 1, ie player[1]
				rowno is 6 to indicate bottom soilders
					   0   1   2   3   4   5   6   7
					 _________________________________
				     0	 |   |   |   |   |   |   |   |   |
					 |___|___|___|___|___|___|___|___|
				     1	 | s | s | s | s | s | s | s | s |
					 |___|___|___|___|___|___|___|___|
				     2	 |   |   |   |   |   |   |   |   |
					 |___|___|___|___|___|___|___|___|
				     3	 |   |   |   |   |   |   |   |   |
					 |___|___|___|___|___|___|___|___|
				     4	 |   |   |   |   |   |   |   |   |
					 |___|___|___|___|___|___|___|___|
				     5	 |   |   |   |   |   |   |   |   |
					 |___|___|___|___|___|___|___|___|
				     6	 | s | s | s | s | s | s | s | s |
					 |___|___|___|___|___|___|___|___|
				     7	 |   |   |   |   |   |   |   |   |
					 |___|___|___|___|___|___|___|___|
			*/

			setcell(rowno,i,'S',player[side]);
			/*
			call setcell to map the pawn
				row:rowno,
				column:i
				'S' as pawn for solider
				player[side] gives the color
			*/
		}
		rowno = side==0?0:7;
		/*
		mapping the remaining pawns, i.e the elephant, knight, bishop
		king and queen will be mapped outside the loop

		the pawn is placed two at a time, since each pawn comes two
		times, once in the left and once in the right
		i=0 will set the left side pawns
		i=1 will set the right side pawns
		the rowno will take care of the row of the pawns,
		when side is 0, row will be 0(top row)
		when side is 1, row will be 7(bottom row)
		*/
		for(i=0;i<2;i++){
			 /*
			 Draw the bishop in cols 2 and 5
			   0   1   2   3   4   5   6   7
			 _________________________________
		0	 |   |   | b |   |   | b |   |   |
			 |___|___|___|___|___|___|___|___|
		1	 | s | s | s | s | s | s | s | s |
			 |___|___|___|___|___|___|___|___|
		2	 |   |   |   |   |   |   |   |   |
			 |___|___|___|___|___|___|___|___|
		3	 |   |   |   |   |   |   |   |   |
			 |___|___|___|___|___|___|___|___|
		4	 |   |   |   |   |   |   |   |   |
			 |___|___|___|___|___|___|___|___|
		5	 |   |   |   |   |   |   |   |   |
			 |___|___|___|___|___|___|___|___|
		6	 | s | s | s | s | s | s | s | s |
			 |___|___|___|___|___|___|___|___|
		7	 |   |   | b |   |   | b |   |   |
			 |___|___|___|___|___|___|___|___|

			 */
			 setcell(rowno,i==0?7-2:2,'B',player[side]);
			//knight
			 setcell(rowno,i==0?7-1:1,'H',player[side]);
			 //Elephant
			 setcell(rowno,i==0?7-0:0,'E',player[side]);

		}
	}
	if(player[1]==BLACK){
		setcell(0,3,'Q',WHITE);
		setcell(0,4,'K',WHITE);
		setcell(7,3,'Q',BLACK);
		setcell(7,4,'K',BLACK);
	}
	else{
		setcell(0,3,'K',BLACK);
		setcell(0,4,'Q',BLACK);
		setcell(7,3,'K',WHITE);
		setcell(7,4,'Q',WHITE);

	}


}

void drawboard(){
	int i,j;
	setcolor(WHITE);
	setfillstyle(SOLID_FILL,WHITE);
	//draw vertical lines
	for(i=startx;i<=endx;i+=cellsize)
		line(i,starty,i,endy);
	//draw horizontal lines
	for(i=starty;i<=endy;i+=cellsize)
		line(startx,i,endx,i);
	//fill colors
	for(i=0;i<8;i++)
		for(j=0;j<8;j++){
			map[i][j].cellcolor=BLACK;
			if(i%2==j%2){
				floodfill(center(col(i)),center(row(j)),WHITE);
				map[i][j].cellcolor=WHITE;
			}
		}

}

void drawcell(int r,int c){
	int i,j,bordercolor,x,y;
	bordercolor=map[r][c].pawncolor==WHITE?DARKGRAY:LIGHTGRAY;
	setcolor(bordercolor);
	setfillstyle(SOLID_FILL,map[r][c].pawncolor);
	x=center(col(c));
	y=center(row(r));

	switch(map[r][c].pawn){
	case 'K':
		rectangle(x-2,y-27,x+2,y-20);
		rectangle(x-8,y-24,x+8,y-22);
	case 'Q':
		rectangle(x-12,y-20,x+12,y-15);
		floodfill(x,y-17,bordercolor);
		rectangle(x-5,y-15,x+5,y+10);
		floodfill(x,y,bordercolor);
		rectangle(x-10,y+10,x+10,y+15);
		floodfill(x,y+12,bordercolor);
		for(i=x-12;i<x+12;i+=6)
			for(j=i;j<i+3;j++)
				line(j,y-20,j,y-17);

		break;
	case 'E':
		rectangle(x-15,y-15,x+15,y-10);
		floodfill(x-14,y-14,bordercolor);
		rectangle(x-10,y-10,x+10,y+10);
		floodfill(x+5,y+5,bordercolor);
		rectangle(x-15,y+10,x+15,y+15);
		floodfill(x-14,y+11,bordercolor);
		for(i=x-15;i<x+15;i+=2)
			line(i,y-15,i,y-13);
		break;
	case 'H':
		rectangle(x+5,y-25,x+10,y-20);
		floodfill(x+7,y-22,bordercolor);
		rectangle(x-20,y-20,x+10,y-5);
		floodfill(x-19,y-14,bordercolor);
		rectangle(x-10,y-5,x+10,y+10);
		floodfill(x+5,y+5,bordercolor);
		rectangle(x-15,y+10,x+15,y+15);
		floodfill(x-14,y+11,bordercolor);
		break;
	case 'S':
		rectangle(x-10,y-15,x+10,y-10);
		floodfill(x,y-12,bordercolor);
		rectangle(x-5,y-10,x+5,y+10);
		floodfill(x,y,bordercolor);
		rectangle(x-10,y+10,x+10,y+15);
		floodfill(x,y+12,bordercolor);
		for(i=x-10;i<x+10;i+=2)
			line(i,y-15,i,y-13);
		break;
	case 'B':
		arc(x,y-15,350,190,10);
		rectangle(x-10,y-15,x+10,y-10);
		floodfill(x,y-12,bordercolor);
		floodfill(x,y-24,bordercolor);
		rectangle(x-5,y-10,x+5,y+10);
		floodfill(x,y,bordercolor);
		rectangle(x-10,y+10,x+10,y+15);
		floodfill(x,y+12,bordercolor);
		for(i=x-10;i<x+10;i+=2)
			line(i,y-15,i,y-13);
		break;

	}


}
void colorcell(int r,int c, int clr){
	setcolor(0);
	setfillstyle(SOLID_FILL,clr);
	bar(col(c),row(r),col(c)+cellsize,row(r)+cellsize);
	rectangle(col(c),row(r),col(c)+cellsize,row(r)+cellsize);
	if(clr!=4)
		map[r][c].cellcolor=clr;

}
int getbc(int r,int c){
	return r%2==c%2?WHITE:BLACK;
}
void selector(int r,int c){
	colorcell(pr,pc,map[pr][pc].cellcolor);
	drawcell(pr,pc);
	if(c==8)c=0;
	else if(c==-1)c=7;
	if(r==8)r=0;
	else if(r==-1)r=7;
	pc=c;
	pr=r;
	colorcell(pr,pc,4);
	drawcell(pr,pc);
}
void render(){
	int i,j;
	char str[100],ch[1];
	for(i=0;i<8;i++)
		for(j=0;j<8;j++)            {
			if(map[i][j].pawn!='N')
				drawcell(i,j);


		}
	selector(pr,pc);



}

void knight(int r,int c){
	if(map[r][c].pawncolor!=turn&&r>=0&&c>=0&&r<8&&c<8)
		colorcell(r,c,3);
}
void select(){
	cell p;
	int i,j,op;
	if(pawnup==0){

		if(turn==map[pr][pc].pawncolor){

			p=map[pr][pc];
			colorcell(pr,pc,12);
			cpr=pr;
			cpc=pc;
			op=turn==0?15:0;
			switch(p.pawn){


				case 'Q':
					for(i=pr+1,j=pc+1;i<8&&j<8;i++,j++)
						if(map[i][j].pawncolor==turn)
							break;
						else if(map[i][j].pawncolor==op){
							colorcell(i,j,3);
							break;
						}
						else
							colorcell(i,j,3);
					for(i=pr+1,j=pc-1;i<8&&j>=0;i++,j--)
						if(map[i][j].pawncolor==turn)
							break;
						else if(map[i][j].pawncolor==op){
							colorcell(i,j,3);
							break;
						}
						else
							colorcell(i,j,3);
					for(i=pr-1,j=pc+1;i<8&&j<8;i--,j++)
						if(map[i][j].pawncolor==turn)
							break;
						else if(map[i][j].pawncolor==op){
							colorcell(i,j,3);
							break;
						}
						else
							colorcell(i,j,3);
					for(i=pr-1,j=pc-1;i<8&&j>=0;i--,j--)
						if(map[i][j].pawncolor==turn)
							break;
						else if(map[i][j].pawncolor==op){
							colorcell(i,j,3);
							break;
						}else
							colorcell(i,j,3);
				case 'E':

					for(i=pr+1;i<8;i++)
						if(map[i][pc].pawncolor==turn){
							break;
							}
						else if(map[i][pc].pawncolor==op){
							colorcell(i,pc,3);
							break;
						}
						else
							colorcell(i,pc,3);

					for(i=pr-1;i>=0;i--)
						if(map[i][pc].pawncolor==turn)
							break;
						else if(map[i][pc].pawncolor==op){
							colorcell(i,pc,3);
							break;
						}
						else
							colorcell(i,pc,3);
					for(i=pc+1;i<8;i++)
						if(map[pr][i].pawncolor==turn)
							break;
						else if(map[pr][i].pawncolor==op){
							colorcell(pr,i,3);
							break;
						}
						else
							colorcell(pr,i,3);
					for(i=pc-1;i>=0;i--)
						if(map[pr][i].pawncolor==turn)
							break;
						else if(map[pr][i].pawncolor==op){
							colorcell(pr,i,3);
							break;
						}
						else
							colorcell(pr,i,3);
					break;

				case 'S':
					if(player[0]==turn){
						if(pr==1&&map[pr+1][pc].pawn=='N')
							colorcell(pr+2,pc,3);
						if(map[pr+1][pc].pawn=='N')
							colorcell(pr+1,pc,3);
						if(map[pr+1][pc+1].pawncolor==op)
							colorcell(pr+1,pc+1,3);
						if(map[pr+1][pc-1].pawncolor==op)
							colorcell(pr+1,pc-1,3);
					}
					else{
						if(pr==6&&map[pr-1][pc].pawn=='N')
							colorcell(pr-2,pc,3);
						if(map[pr-1][pc].pawn=='N')
							colorcell(pr-1,pc,3);
						if(map[pr-1][pc-1].pawncolor==op)
							colorcell(pr-1,pc-1,3);
						if(map[pr-1][pc+1].pawncolor==op)
							colorcell(pr-1,pc+1,3);
					}


					break;
				case 'K':
					if(map[pr-1][pc-1].pawncolor!=turn)
						colorcell(pr-1,pc-1,3);
					if(map[pr-1][pc].pawncolor!=turn)
						colorcell(pr-1,pc,3);
					if(map[pr-1][pc+1].pawncolor!=turn)
						colorcell(pr-1,pc+1,3);
					if(map[pr][pc-1].pawncolor!=turn)
						colorcell(pr,pc-1,3);
					if(map[pr][pc+1].pawncolor!=turn)
						colorcell(pr,pc+1,3);
					if(map[pr+1][pc-1].pawncolor!=turn)
						colorcell(pr+1,pc-1,3);
					if(map[pr+1][pc].pawncolor!=turn)
						colorcell(pr+1,pc,3);
					if(map[pr+1][pc+1].pawncolor!=turn)
						colorcell(pr+1,pc+1,3);
					break;
				case 'B':
					for(i=pr+1,j=pc+1;i<8&&j<8;i++,j++)
						if(map[i][j].pawncolor==turn)
							break;
						else if(map[i][j].pawncolor==op){
							colorcell(i,j,3);
							break;
						}
						else
							colorcell(i,j,3);
					for(i=pr+1,j=pc-1;i<8&&j>=0;i++,j--)
						if(map[i][j].pawncolor==turn)
							break;
						else if(map[i][j].pawncolor==op){
							colorcell(i,j,3);
							break;
						}else
							colorcell(i,j,3);
					for(i=pr-1,j=pc+1;i<8&&j<8;i--,j++)
						if(map[i][j].pawncolor==turn)
							break;
						else if(map[i][j].pawncolor==op){
							colorcell(i,j,3);
							break;
						}
						else
							colorcell(i,j,3);
					for(i=pr-1,j=pc-1;i<8&&j>=0;i--,j--)
						if(map[i][j].pawncolor==turn)
							break;
						else if(map[i][j].pawncolor==op){
							colorcell(i,j,3);
							break;
						}else
							colorcell(i,j,3);
					break;
				case 'H':
					if(map[pr-2][pc-1].pawncolor!=turn)
						knight(pr-2,pc-1);
					if(map[pr-2][pc+1].pawncolor!=turn)
						knight(pr-2,pc+1);
					if(map[pr-1][pc-2].pawncolor!=turn)
						knight(pr-1,pc-2);
					if(map[pr-1][pc+2].pawncolor!=turn)
						knight(pr-1,pc+2);
					if(map[pr+1][pc-2].pawncolor!=turn)
						knight(pr+1,pc-2);
					if(map[pr+1][pc+2].pawncolor!=turn)
						knight(pr+1,pc+2);
					if(map[pr+2][pc-1].pawncolor!=turn)
						knight(pr+2,pc-1);
					if(map[pr+2][pc+1].pawncolor!=turn)
						knight(pr+2,pc+1);

			}
			pawnup=1;
		}
	}
	else{

		if(map[pr][pc].cellcolor==3)
			{
			if(map[pr][pc].pawn=='K')
				exit(0);
			map[pr][pc].pawn=map[cpr][cpc].pawn;
			map[pr][pc].pawncolor=map[cpr][cpc].pawncolor;
			if(map[pr][pc].pawn=='S'&&(pr==0||pr==7))
				map[pr][pc].pawn='Q';
			drawcell(pr,pc);
			clearcell(cpr,cpc);
			turn=turn==0?15:0;

			setcolor(0);
			setfillstyle(SOLID_FILL,0);
			bar(0,0,80,80);
			setcolor(15);
			if(turn==15)
				outtextxy(10,0,"White's");
			else
				outtextxy(10,0,"Black's");
			outtextxy(10,20,"Turn");

			}
		colorcell(cpr,cpc,getbc(cpr,cpc));
		for(i=0;i<8;i++)
			for(j=0;j<8;j++)
				if(map[i][j].cellcolor==3)
					colorcell(i,j,getbc(i,j));
		pawnup=0;
	}
}

int cellstate(int r, int c){
	return map[r][c].cellcolor;
}
void ai(){
	int flag=0,r,c;
	while(flag==0){
		r=rand()%8;
		c=rand()%8;
		if(map[r][c].pawn!='N'&&map[r][c].pawncolor==turn)
			flag=1;
	}
	selector(r,c);
	flag=0;
	while(flag==0){
		r=rand()%8;
		c=rand()%8;
		if(cellstate(r,c)==4)
			flag=1;
	}
	select();
	turn=15;
}
void start(){
	int ch,i,j;
	srand(time(NULL));
	init();
	pc=4;
	pr=4;

	drawboard();
	setcolor(10);
	setfillstyle(SOLID_FILL,0);
	bar(0,0,80,80);
	setcolor(15);
	if(turn==15)
		outtextxy(10,0,"White's");
	else
		outtextxy(10,0,"Black's");
	outtextxy(10,20,"Turn");
	render();
	while(1){
		if(kbhit()){
			ch = getch();
			switch(ch){
			case 13:select();break;
			case 27:cleardevice();
				exit(0);
			case 77: selector(pr,pc+1);break;
			case 75: selector(pr,pc-1);break;
			case 72: selector(pr-1,pc);break;
			case 80: selector(pr+1,pc);break;
			}


			render();
		}
	}

}
void main(){
	int	graphdevice,
		graphmode,
		side,
		i,j,
		rowno;
	graphdevice = DETECT;//Detect screen size
	pawnup=0;
	initgraph(&graphdevice,&graphmode,"C:/TC/BGI");
	setcolor(15);
	rectangle(320-100,100,320+100,200);
	floodfill(320,150,15);
	getch();
   //start();

}
