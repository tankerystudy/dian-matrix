######################################################################
# Edit by Tankery, Fri Oct 1 2010
# This is the project file is the application project
######################################################################

# basic configuration
PROJECT     = DianMatrix
TEMPLATE    = app
DEPENDPATH  += . \
               ../../DianMatrix \
               ../../dian_matrix_lib
INCLUDEPATH += ../../dian_matrix_lib
LIBPATH     += ../dian_matrix_lib/lib \
               ../qserialdevice/lib

CONFIG      += qt warn_on
CONFIG(debug, debug|release) {
    LIBS    += -ldian_matrix_libd \
                -lqserialdeviced
} else {
    LIBS    += -ldian_matrix_lib \
                -lqserialdevice
}

# Input
SOURCES     += DianMatrix.cpp

# Output
TARGET       = 
DESTDIR      = ./bin
OBJECTS_DIR  = ./obj
MOC_DIR      = ./moc

