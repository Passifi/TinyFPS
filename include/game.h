
void  handleInput();
void handleScripts();
void handleGameLogic(); 
void  constructRenderList();
void renderScene();
void updateMusic();
void handlePhysics(); 

void game_loop() {
    handleInput();
    handleGameLogic(); // timing, scripting, statemanagement
    constructRenderList();
    renderScene();
    updateMusic();
    handlePhysics(); 
};