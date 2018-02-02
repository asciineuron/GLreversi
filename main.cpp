#include <iostream>
#include "glad/glad.h"
#include <GLFW/glfw3.h>
#include <fstream> //interesting to add a function to write a log file to record game outcomes
#include <utility>
#include <string>
#include <thread>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include "board.h"

using namespace std;

//functions:
void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void cursor_position_callback(GLFWwindow *window, double xpos, double ypos); //for finding mouse position
void processInput(GLFWwindow *window, Board *board);
pair<int, int> current_score(); //first element # white disks, second # black disks
pair<int, int> string_to_move(string input); //parses string into board position, e.g. E3 -> (4,2)
void parse_input(Board *board); //will parse user input and break down into further commands (e.g. string_to_move)
void check_end();
void quit(); //sequence of quit events
void next_turn(); //simply flips player_to_turn
void render_thread(Board *board);
pair<int, int> click_to_move(double *xpos, double *ypos);
//disk values:
const int WHITE = 1;
const int BLACK = -1;
const int EMPTY = 0;

int BOARD_HEIGHT = 800;
int BOARD_WIDTH = 600; //will update with size of board

int board[8][8]; //board 8x8 array with disk representation (1 for white, -1 for black, no objects needed!)
//rows: 1-8, columns: A-H

bool is_end = false; //true once neither player has valid move
int player_to_move = WHITE;
pair<int, int> score;
pair<bool, bool> has_move; //records if each player has a valid move (game end when both false), first white, second black
string input;

const char *vertexShaderSource =
	"#version 330 core\n"
	"layout (location = 0) in vec3 aPos;\n"
	"layout (location = 1) in vec2 aTexCoord;\n"
	"out vec2 TexCoord;\n"
	"uniform vec2 positionScale;\n"
	"void main()\n"
	"{\n"
	"	gl_Position = vec4(aPos.x + positionScale.x, aPos.y - positionScale.y, aPos.z, 1.0);\n"
	"	TexCoord = aTexCoord;\n"
	"}\0";

const char *fragmentShaderSource =
	"#version 330 core\n"
	"out vec4 FragColor;\n"
	"in vec2 TexCoord;\n"
	"uniform sampler2D ourTexture;\n"
	"void main()\n"
	"{\n"
	"	FragColor = texture(ourTexture, TexCoord);\n"
	"}\0";


int main()
{
	has_move.first = true;
	has_move.second = true;

    Board board;
    /*
	//starting positions of disks:
	board[3][3] = WHITE;
	board[4][4] = WHITE;
	board[3][4] = BLACK;
	board[4][3] = BLACK;
*/
	cout << "Welcome to reversi!" << endl;
	score = current_score(); //these lines set up initial view correctly	
	board.print_data(score, player_to_move);
  
   // std::thread getinput(parser_thread);
	std::thread getrender(render_thread, &board);
    
    while (!is_end)
    {
        /*
        score = current_score();
		check_end(); //checks if either player has move, if not then game over
		if (is_end) break;
		parse_input(&board);
		if (is_end) break; //try to cleanup later, check at each point game might end
		next_turn();
		score = current_score();
        board.print_data(score, player_to_move);
        */
    }
    //getinput.join();
	getrender.join();
    
	return 0;

}


void parse_input(Board *board)
{
	RETRY: //returns to here if incorrect input
	cin >> input; //reads first word of input
	//getline(cin, input);
	if (input == "quit")
	{
		quit();	
	} else if (input.find("pass") != std::string::npos)
	{
		if (player_to_move == WHITE)
		{
			has_move.first = false;
		} else if (player_to_move == BLACK)
		{
			has_move.second = false;
		}
	} else if (input.find("play") != std::string::npos)
	{
		cin >> input; //takes second word of input (movement position)
		if (board->is_valid_move(string_to_move(input), player_to_move))
		{
			board->enact_move(string_to_move(input), player_to_move);	
		} else
		{
			cout << "not valid move, please try again: ";
			goto RETRY;
		}
	}
	
}


void next_turn()
{
	player_to_move *= -1;
}

void quit()
{
	is_end = true;
	ofstream log;
	log.open("game_log.txt", ios::app); //append to end of file
	if (score.first > score.second)
	{
		log << "White\n";
	} else if (score.second > score.first)
	{
		log << "Black\n";
	} else if (score.first == score.second)
	{
		log << "Tie\n";
	}
	
	cout << "Game over, thank you for playing!" << endl;
	//goto QUIT;
}

void check_end()
{
	if (has_move.first == false && has_move.second == false)
	{
		quit();
	}
}


pair<int, int> current_score()
{
	pair<int, int> score; //first white, second black
	for (int row = 0; row < sizeof(board)/sizeof(*board); row++)
	{
		for (int col = 0; col < sizeof(board[row])/sizeof(*board[row]); col++)
		{
			if (board[row][col] == WHITE)
			{
				score.first++;
			} else if (board[row][col] == BLACK)
			{
				score.second++;
			}
		}
	}
	return score;
}

pair<int, int> string_to_move(string position_input)
{
	pair<int, int> position;
	//parse col/letter into num:
	if (position_input[0] == 'a')
	{
		position.first = 0;
	} else if (position_input[0] == 'b')
	{
		position.first = 1;
	} else if (position_input[0] == 'c')
	{
		position.first = 2;
	} else if (position_input[0] == 'd')
	{
		position.first = 3;
	} else if (position_input[0] == 'e')
	{
		position.first = 4;
	} else if (position_input[0] == 'f')
	{
		position.first = 5;
	} else if (position_input[0] == 'g')
	{
		position.first = 6;
	} else if (position_input[0] == 'h') 
	{
		position.first = 7;
	}
	
	//parse row/num into num
	position.second = position_input[1] - '0' - 1; //convert char to int, subtract 1 to match board numbering
	return position;
}

void render_thread(Board *board)
{
   
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow *window = glfwCreateWindow(800, 800, "Reversi", NULL, NULL);
	if (window == NULL)
	{
		cout << "Failed to create window" << endl;
		glfwTerminate();
		//return -1;	
	}

	glfwMakeContextCurrent(window);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		cout << "Failed to initialize GLAD" << endl;
		//return -1;
	}

	glViewport(0, 0, 800, 600);

	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback); 
    //glfwSetCursorPosCallback(window, cursor_position_callback);
    
    
	float vertices[] =
	{
		//x, y, z - texture coords
		//two for square
		-1.0f, 1.0f, 0.0f, 0.0f, 1.0f,//top left
		-0.75f, 1.0f, 0.0f, 1.0f, 1.0f,//top right
		-0.75f, 0.75f, 0.0f, 1.0f, 0.0f,//bottom right
		-1.0f, 0.75f, 0.0f, 0.0f, 0.0f//bottom left
	};

	unsigned int indices[] =
	{
		0, 1, 2,
		0, 2, 3
	};

	unsigned int VBO;
	glGenBuffers(1, &VBO);
	
    unsigned int EBO;
	glGenBuffers(1, &EBO);

	unsigned int vertexShader;
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL); //attach vertex shader source to vertex shader objects
	glCompileShader(vertexShader);

	int success;
	char infoLog[512];
	
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success); //error detection for shader creation
	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl; //report errors, if any
	}

	unsigned int fragmentShader;
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL); //attach fragment shader source to fragment shader object
	glCompileShader(fragmentShader);

	unsigned int shaderProgram; //combination of vertex and fragment shaders
	shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);

	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog); //error detection and report for program creation
	}
	
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader); //delete shaders once incorporated into program

	unsigned int VAO;
	glGenVertexArrays(1, &VAO); //generate vertex array

	glBindVertexArray(VAO); //bind vertex array

	glBindBuffer(GL_ARRAY_BUFFER, VBO); //copy vertices array in VBO buffer for OpenGL to use
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	//first for position coords
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0); //set vertex attributes pointer #0
	glEnableVertexAttribArray(0);

	//second for texture coords
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3*sizeof(float)));
	glEnableVertexAttribArray(1);

	//black tile texture
	unsigned int texture_black;
    	glGenTextures(1, &texture_black);
    	glBindTexture(GL_TEXTURE_2D, texture_black); // all upcoming GL_TEXTURE_2D operations now have effect on this texture object
    	// load image, create texture and generate mipmaps
    	int width, height, nrChannels;
    	// The FileSystem::getPath(...) is part of the GitHub repository so we can find files on any IDE/platform; replace it with your own image path.
    	unsigned char *data = stbi_load("black_disk.png\0", &width, &height, &nrChannels, STBI_rgb_alpha);
    	if (data)
    	{
    	    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    	    glGenerateMipmap(GL_TEXTURE_2D);
    	}
    	else
    	{
    	    std::cout << "Failed to load texture" << std::endl;
    	}
    	stbi_image_free(data);

	//white
	unsigned int texture_white;
	glGenTextures(1, &texture_white);
	glBindTexture(GL_TEXTURE_2D, texture_white);
	data = stbi_load("white_disk.png\0", &width, &height, &nrChannels, STBI_rgb_alpha);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
	glGenerateMipmap(GL_TEXTURE_2D);
	stbi_image_free(data);
	
	//empty
	unsigned int texture_empty;
	glGenTextures(1, &texture_empty);
	glBindTexture(GL_TEXTURE_2D, texture_empty);
	data = stbi_load("empty_disk.png\0", &width, &height, &nrChannels, STBI_rgb_alpha);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
	glGenerateMipmap(GL_TEXTURE_2D);
	stbi_image_free(data);

    while(!glfwWindowShouldClose(window))
	{
        check_end(); //checks if either player has move, if not then game over
		if (is_end) break;
        
		//input
		processInput(window, board);
        
        if (is_end) break; //try to cleanup later, check at each point game might end
		//next_turn();
        score = current_score();
        
		//rendering commands
		//glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClearColor(0.5f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT); //clears the color of buffer each iteration, filling with color specified in glClearColor

		glBindTexture(GL_TEXTURE_2D, texture_empty);
		glUseProgram(shaderProgram); //activate
		glBindVertexArray(VAO);
		//glDrawArrays(GL_TRIANGLES, 0, 3); used for VBO
		int positionScaleLocation = glGetUniformLocation(shaderProgram, "positionScale");
		if (!is_end)
		{
		for (int i = 0; i < sizeof((board->data))/sizeof(*(board->data)); i++)
		{
			for (int j = 0; j < sizeof((board->data[i]))/sizeof(*(board->data[i])); j++)
			{
				if (board->data[i][j] == WHITE)
				{
					glBindTexture(GL_TEXTURE_2D, texture_white);
				} else if (board->data[i][j] == BLACK)
				{
					glBindTexture(GL_TEXTURE_2D, texture_black);
				} else
				{
					glBindTexture(GL_TEXTURE_2D, texture_empty);
				}
				glUniform2f(positionScaleLocation, 0.25f*j, 0.25f*i);
				glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
			}
		}
		}
		
		
		//parse_input();
		
		
        board->print_data(score, player_to_move);
		
		
		//check and call events and swap the buffers 
    		glfwSwapBuffers(window);
    		glfwWaitEvents();
            //glfwPollEvents();
           
	}
    glfwTerminate();
    
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(-1, 0, width, height);
    BOARD_HEIGHT = height;
    BOARD_WIDTH = width;
}

void cursor_position_callback(GLFWwindow* window, double xpos, double ypos)
{

    
}

pair<int, int> click_to_move(double *xpos, double *ypos)
{
    pair<int, int> position;
    position.first = -1;
    position.second = -1;
    //column, pair.first
    if (*xpos <= (1.0f/8.0f) * BOARD_WIDTH)
    {
        position.first = 0;
    } else if (*xpos <= (2.0f/8.0f)* BOARD_WIDTH)
    {
        position.first = 1;
    } else if (*xpos <= (3.0f/8.0f)* BOARD_WIDTH)
    {
        position.first = 2;
    } else if (*xpos <= (4.0f/8.0f)* BOARD_WIDTH)
    {
        position.first = 3;
    } else if (*xpos <= (5.0f/8.0f)* BOARD_WIDTH)
    {
        position.first = 4;
    } else if (*xpos <= (6.0f/8.0f)* BOARD_WIDTH)
    {
        position.first = 5;
    } else if (*xpos <= (7.0f/8.0f)* BOARD_WIDTH)
    {
        position.first = 6;
    } else if (*xpos <= 1.0f* BOARD_WIDTH)
    {
        position.first = 7;
    }
    //row
    if (*ypos <= (1.0f/8.0f) * BOARD_HEIGHT)
    {
        position.second = 0;
    } else if (*ypos <= (2.0f/8.0f)* BOARD_HEIGHT)
    {
        position.second = 1;
    } else if (*ypos <= (3.0f/8.0f)* BOARD_HEIGHT)
    {
        position.second = 2;
    } else if (*ypos <= (4.0f/8.0f)* BOARD_HEIGHT)
    {
        position.second = 3;
    } else if (*ypos <= (5.0f/8.0f)* BOARD_HEIGHT)
    {
        position.second = 4;
    } else if (*ypos <= (6.0f/8.0f)* BOARD_HEIGHT)
    {
        position.second = 5;
    } else if (*ypos <= (7.0f/8.0f)* BOARD_HEIGHT)
    {
        position.second = 6;
    } else if (*ypos <= 1.0f* BOARD_HEIGHT)
    {
        position.second = 7;
    }
    return position;
}

void processInput(GLFWwindow *window, Board *board)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        	glfwSetWindowShouldClose(window, true); //sets window to close if user presses ESC
            
    //test for mouse click input
    double xpos, ypos;
    glfwGetCursorPos(window, &xpos, &ypos);
    //cout << "x " << xpos << " y " << ypos << endl;
    int state = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT);
    if (state == GLFW_PRESS)
    {
        if (board->is_valid_move(click_to_move(&xpos, &ypos), player_to_move))
		{
			board->enact_move(click_to_move(&xpos, &ypos), player_to_move);
            next_turn();
		}
       
    }
}
