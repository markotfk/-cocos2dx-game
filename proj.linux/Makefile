EXECUTABLE = rally

SOURCES = main.cpp \
        ../Classes/AppDelegate.cpp \
        ../Classes/TrackScene.cpp \
        ../Classes/CarTire.cpp \
        ../Classes/RaceCar.cpp \
        ../Classes/ContactListener.cpp \
        ../Classes/FixtureUserData.cpp \
        ../Classes/GroundAreaFUD.cpp \
        ../Classes/DebugDraw.cpp

COCOS_ROOT = ../../..
INCLUDES = -I../Classes -I$(COCOS_ROOT)/cocos2dx/
INCLUDES += -I$(COCOS_ROOT)/CocosDenshion/include

include $(COCOS_ROOT)/cocos2dx/proj.linux/cocos2dx.mk
SHAREDLIBS += -lcocos2d -lcocosdenshion -lbox2d -lglut -lGL -lglfw3 -lGLU -lX11 -lXxf86vm -lXrandr -lpthread -lXi
COCOS_LIBS = $(LIB_DIR)/libcocos2d.so
COCOS_LIBS += $(LIB_DIR)/libcocosdenshion.so
COCOS_LIBS += $(LIB_DIR)/libbox2d.a

$(TARGET): $(OBJECTS) $(STATICLIBS) $(COCOS_LIBS) $(CORE_MAKEFILE_LIST)
	@mkdir -p $(@D)
	$(LOG_LINK)$(CXX) $(CXXFLAGS) $(OBJECTS) -o $@ $(SHAREDLIBS) $(STATICLIBS)

$(OBJ_DIR)/%.o: %.cpp $(CORE_MAKEFILE_LIST)
	@mkdir -p $(@D)
	$(LOG_CXX)$(CXX) $(CXXFLAGS) $(INCLUDES) $(DEFINES) $(VISIBILITY) -c $< -o $@

$(OBJ_DIR)/%.o: ../%.cpp $(CORE_MAKEFILE_LIST)
	@mkdir -p $(@D)
	$(LOG_CXX)$(CXX) $(CXXFLAGS) $(INCLUDES) $(DEFINES) $(VISIBILITY) -c $< -o $@
