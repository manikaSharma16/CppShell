CC = g++
CFLAGS = -std=c++11 -Wall -g -I./include  # Added -I./include to tell the compiler where to find the headers

# For macOS, if libcurl is installed via Homebrew, specify the library path
LIBCURL_FLAGS = -L/usr/local/opt/curl/lib -lcurl  # Link to libcurl library (update path if necessary)

SRC = src/main.cpp src/shell.cpp src/command_history.cpp src/system_commands.cpp src/custom_commands.cpp
OBJ = $(SRC:.cpp=.o)
EXEC = shell

all: $(EXEC)

$(EXEC): $(OBJ)
	$(CC) $(CFLAGS) $(LIBCURL_FLAGS) -o $(EXEC) $(OBJ)  # Added $(LIBCURL_FLAGS) to link curl

%.o: %.cpp
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) $(EXEC)