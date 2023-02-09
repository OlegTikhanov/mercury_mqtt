ifneq ($V,1)
Q ?= @
endif

CC	= gcc
LDLIBS    =  -lmosquitto -lpthread

###############################################################################

SRC	=	main.c mercury.c mqtt.c
BIN	=	mercury

main:
	$Q echo [link]
	$Q $(CC) $(SRC) -o $(BIN) $(LDLIBS)

run: 
	$Q ./$(BIN)

install:
	$Q echo [install]
	$Q sudo cp $(BIN).service /etc/systemd/system/
	$Q sudo cp $(BIN) /usr/local/bin/
	$Q sudo systemctl daemon-reload
	$Q sudo systemctl enable $(BIN).service
	$Q sudo systemctl start $(BIN).service

uninstall:
	$Q echo [uninstall]
	$Q sudo systemctl stop $(BIN).service
	$Q sudo rm /usr/local/bin/$(BIN)
	$Q sudo rm /etc/systemd/system/$(BIN).service
	$Q sudo systemctl daemon-reload

clean:
	$Q echo "[Clean]"
	$Q rm -f $(BIN)