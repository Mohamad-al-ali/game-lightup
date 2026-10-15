Module.onRuntimeInitialized = () => { start();  }


/**   ----------------Mouse part--------------  **/
var p1 = document.getElementById("p1");
var d1 = document.getElementById("d1");

var walls_id = document.getElementById("walls_id");
var walls_nb = document.getElementById("walls_nb");

d1.addEventListener("mouseover", () => {p1.style.display = "block";});
d1.addEventListener("mouseout", () => {p1.style.display = "none";});

walls_id.addEventListener("mouseover", () => {walls_nb.style.display = "block";});
walls_id.addEventListener("mouseout", () => {walls_nb.style.display = "none";});
/**   ----------------Mouse part--------------  **/

/**   ----------------img part--------------  **/
var lightbulb_IMG = new Image();
lightbulb_IMG.src = "lightbulb.png";
var lightbulb_IMG_error = new Image();
lightbulb_IMG_error.src = "lightbulbE.png";
/**   ----------------img part--------------  **/


var canvasPerso = document.getElementById('mycanvasPerso');
canvasPerso.addEventListener('click', canvasLeftClickP);
  


var sX;
var sY;
var game;
var game_s;
var nb_w;
var warpping;
var nb_col=7;
var nb_row=7;
var cell_s;



function canvasLeftClickP(event){
    event.preventDefault(); // prevent default context menu to appear...
    // get relative cursor position in canvas
    console.log("left click at position:", event.offsetX, event.offsetY);
    sX = event.offsetX;
    sY = event.offsetY;
    start1();
}



function printGame(g) {
    var nb_rows = Module._nb_rows(g);
    var nb_cols = Module._nb_cols(g);
    var begin_cell_size=64-(nb_rows+nb_cols);
    cell_s=begin_cell_size;

    var ctxP = canvasPerso.getContext('2d');
    canvasPerso.width=begin_cell_size*nb_cols;
    canvasPerso.height=begin_cell_size*nb_rows;
    var widthP = canvasPerso.width;
    var heightP = canvasPerso.height;
    
    // ctxP.clearRect(0, 0, widthP, heightP);
    var cell_size=Math.min(widthP/nb_cols,heightP/nb_rows);
    // draw a yellow square at (squareX, squareY) position
    ctxP.save();
    var fontSize= begin_cell_size-20;
    ctxP.font = fontSize+'px Arial';
    ctxP.textBaseline = 'middle';
    ctxP.textAlign = 'center';
    
    for (var row = 0; row < nb_rows; row++) {
        for (var col = 0; col < nb_cols; col++) {
            var blank = Module._is_blank(g, row, col);
            var black = Module._is_black(g, row, col);
            var ligthed = Module._is_lighted(g, row, col);
            var lightbulb = Module._is_lightbulb(g, row, col);
            var marked = Module._is_marked(g, row, col);
            var error = Module._has_error(g, row, col);

            if(black){
                ctxP.fillStyle = 'black';
                ctxP.fillRect(col*cell_size,row*cell_size,  begin_cell_size, begin_cell_size);
                var Nb_black = Module._get_black_number(g, row, col);
                if(error)
                    ctxP.fillStyle = 'red';
                else
                    ctxP.fillStyle = 'white';
                if(Nb_black==0){
                    ctxP.fillText("0",col*cell_size+(cell_size/2) ,row*cell_size+(cell_size/2));
                }else if(Nb_black==1){
                    ctxP.fillText("1",col*cell_size+(cell_size/2) ,row*cell_size+(cell_size/2));
                }else if(Nb_black==2){
                    ctxP.fillText("2",col*cell_size+(cell_size/2) ,row*cell_size+(cell_size/2));
                }else if(Nb_black==3){
                    ctxP.fillText("3",col*cell_size+(cell_size/2) ,row*cell_size+(cell_size/2));
                }else if(Nb_black==4){
                    ctxP.fillText("4",col*cell_size+(cell_size/2) ,row*cell_size+(cell_size/2));
                }
            }else if(lightbulb){
                ctxP.fillStyle = 'yellow';
                ctxP.fillRect(col*cell_size,row*cell_size,  begin_cell_size, begin_cell_size);

                if (error) 
                    ctxP.drawImage(lightbulb_IMG_error, col*cell_size,row*cell_size, begin_cell_size, begin_cell_size);
                else 
                    ctxP.drawImage(lightbulb_IMG, col*cell_size,row*cell_size, begin_cell_size, begin_cell_size);

            }else if(ligthed){
                ctxP.fillStyle = 'yellow';
                ctxP.fillRect(col*cell_size,row*cell_size,  begin_cell_size, begin_cell_size);
            }
             if(marked){
                ctxP.fillStyle = 'red';
                ctxP.fillText("-",col*cell_size+(cell_size/2) ,row*cell_size+(cell_size/2));
            }
            
            ctxP.moveTo(0, row*cell_size);
            ctxP.lineTo(widthP, row*cell_size);
            ctxP.moveTo(col*cell_size, 0);
            ctxP.lineTo(col*cell_size, heightP);
        }
    }
    ctxP.stroke(); // the stroke will actually paint the current lines
    ctxP.restore();

}

function update(g,sY,sX){
    if(sX != null && sY != null){
        if( Module._is_blank(g,sY/cell_s,sX/cell_s)){
            Module._play_move(g, sY/cell_s,sX/cell_s, 1);
        }else 
        if(Module._is_lightbulb(g,sY/cell_s,sX/cell_s)){
            Module._play_move(g, sY/cell_s,sX/cell_s, 0);
        }
    }
}

function start1(){
    update(game,sY,sX);  
    printGame(game);
}

function start() {
    console.log("call start routine");
    var g = Module._copy(Module._new_random(7,7,false,(((1/100)*20)*(7*7)),true));
    game_s=Module._copy(g);
    Module._restart(g);
    game=Module._copy(g);
    start1();
    Module._delete(g);
}



//solve function
function solve_b(){
    game=Module._copy(game_s)
    printGame(game);
}

//restart function
function restart_b(){
    Module._restart(game);
    printGame(game);
}


//Random_fixed
function Random_f(elm) {
    var id=document.getElementById(elm);
	var nb=id.dataset.index;
    console.log(nb);
    Module._delete(game);
    if(nb_w==0 || nb_w==null){
        nb_w=(nb*nb)*0.3;
    }
    game=Module._copy(Module._new_random(nb,nb,warpping,(((1/100)*nb_w)*(nb*nb)),true));
    game_s=Module._copy(game);
    Module._restart(game);
    printGame(game)
}

//walls
function walls(elm) {
    var id=document.getElementById(elm);
    if(id==null){
        nb_w=0.0;    
    }
	nb_w=parseInt(id.value);
    
}

//undo function
function undo_b(){
    Module._undo(game);
    printGame(game);
}

//redo function
function redo_b(){
    Module._redo(game);
    printGame(game);
}


//warp
function warp(elm) {
    var id=document.getElementById(elm);
    if(id==null){
        warpping=false;    
    }
	id.addEventListener('change', (event) => {
        if (event.currentTarget.checked) {
            warpping=true;
        } else {
          warpping=false;
        }
      });
    
}

//Random_custom
function Random_c() {
    Module._delete(game);
    if(nb_w==0 || nb_w==null){
        nb_w=50;
    }
    game=Module._copy(Module._new_random(nb_col,nb_row,warpping,(((1/100)*nb_w)*(nb_col*nb_row)),true));
    game_s=Module._copy(game);
    Module._restart(game);
    printGame(game)
}

function col(elm){
    var id=document.getElementById(elm);
    nb_col =parseInt(id.value);
    console.log(nb_col)
}

function row(elm){
    var id=document.getElementById(elm);
    nb_row =parseInt(id.value);
    console.log(nb_row);

}