# Makefile para compilar el proyecto

# Definición de variables
HOME = .
BIN = $(HOME)/bin
INCLUDE = $(HOME)/include
SRC = $(HOME)/src
OBJ = $(HOME)/obj

all: \
preambulo \
ejecutables \
$(BIN)/main_BLocalParalela \
$(BIN)/EvolutivoGeneracionalParalelo \
fin-ejecutables

preambulo:
	@echo
	@echo -----------------------------------------------------------
	@echo TRABAJO FIN DE GRADO
	@echo
	@echo "("c")" Cristóbal Pérez Simón
	@echo Archivos para la realización del TFG
	@echo Universidad de Granada. Curso 2023-2024
	@echo -----------------------------------------------------------
	@echo

ejecutables:
	@echo
	@echo Creando ejecutables...
fin-ejecutables:
	@echo
	@echo ...Ejecutables creados
	@echo

#Objetos ....................................
$(OBJ)/z_io.o: $(SRC)/z_io.cpp $(INCLUDE)/z_io.h $(INCLUDE)/z_output.h \
		       $(INCLUDE)/z_Individuo.h 
	
	g++ -g -c -o $(OBJ)/z_io.o $(SRC)/z_io.cpp -I$(INCLUDE) -std=c++11

$(OBJ)/z_output.o: $(SRC)/z_output.cpp $(INCLUDE)/z_output.h $(INCLUDE)/z_Individuo.h \
				   $(INCLUDE)/z_io.h
	
	g++ -g -c -o $(OBJ)/z_output.o $(SRC)/z_output.cpp -I$(INCLUDE) -std=c++11 

$(OBJ)/z_Individuo.o: $(SRC)/z_Individuo.cpp $(INCLUDE)/z_Individuo.h $(INCLUDE)/z_io.h
	
	g++ -g -c -o $(OBJ)/z_Individuo.o $(SRC)/z_Individuo.cpp -I$(INCLUDE) -std=c++11

$(OBJ)/Paralelismo.o: $(SRC)/Paralelismo.cu $(INCLUDE)/Paralelismo.h 
	
	nvcc -G -g -c -o $(OBJ)/Paralelismo.o $(SRC)/Paralelismo.cu -I$(INCLUDE) -std=c++11

$(OBJ)/Paralelismo_OMP.o: $(SRC)/Paralelismo_OMP.cpp $(INCLUDE)/Paralelismo_OMP.h 
	
	g++ -openmp -g -c -o $(OBJ)/Paralelismo_OMP.o $(SRC)/Paralelismo_OMP.cpp -I$(INCLUDE) -std=c++11

$(OBJ)/FuncionesBLocalParalela.o: $(SRC)/FuncionesBLocalParalela.cpp $(INCLUDE)/FuncionesBLocalParalela.h \
									$(INCLUDE)/z_io.h $(INCLUDE)/z_output.h \
						 			$(INCLUDE)/z_Individuo.h $(INCLUDE)/Paralelismo.h $(INCLUDE)/Paralelismo_OMP.h
	
	nvcc -G -Xcompiler -openmp -g -c -o $(OBJ)/FuncionesBLocalParalela.o $(SRC)/FuncionesBLocalParalela.cpp -I$(INCLUDE) -std=c++11

$(OBJ)/main_BLocalParalela.o: $(SRC)/main_BLocalParalela.cpp $(INCLUDE)/FuncionesBLocalParalela.h 
	
	nvcc -G -Xcompiler -openmp -g -c -o $(OBJ)/main_BLocalParalela.o $(SRC)/main_BLocalParalela.cpp -I$(INCLUDE) -std=c++11

$(OBJ)/EvolutivoGeneracionalParalelo.o: $(SRC)/EvolutivoGeneracionalParalelo.cpp $(INCLUDE)/FuncionesBLocalParalela.h
	
	nvcc -G -Xcompiler -openmp -g -c -o $(OBJ)/EvolutivoGeneracionalParalelo.o $(SRC)/EvolutivoGeneracionalParalelo.cpp -I$(INCLUDE) -std=c++11

#Ejecutables ....................................
$(BIN)/main_BLocalParalela: $(OBJ)/main_BLocalParalela.o $(OBJ)/FuncionesBLocalParalela.o \
							$(OBJ)/z_io.o $(OBJ)/z_output.o $(OBJ)/z_Individuo.o $(OBJ)/Paralelismo.o $(OBJ)/Paralelismo_OMP.o
								
	nvcc -G -Xcompiler -g -o $(BIN)/main_BLocalParalela $(OBJ)/main_BLocalParalela.o \
								$(OBJ)/FuncionesBLocalParalela.o $(OBJ)/z_io.o $(OBJ)/z_output.o \
								$(OBJ)/z_Individuo.o $(OBJ)/Paralelismo.o $(OBJ)/Paralelismo_OMP.o -I$(INCLUDE) -std=c++11 

$(BIN)/EvolutivoGeneracionalParalelo: $(OBJ)/EvolutivoGeneracionalParalelo.o $(OBJ)/FuncionesBLocalParalela.o \
							$(OBJ)/z_io.o $(OBJ)/z_output.o $(OBJ)/z_Individuo.o $(OBJ)/Paralelismo.o $(OBJ)/Paralelismo_OMP.o
								
	nvcc -G -Xcompiler -g -o $(BIN)/EvolutivoGeneracionalParalelo $(OBJ)/EvolutivoGeneracionalParalelo.o \
								$(OBJ)/FuncionesBLocalParalela.o $(OBJ)/z_io.o $(OBJ)/z_output.o \
								$(OBJ)/z_Individuo.o $(OBJ)/Paralelismo.o $(OBJ)/Paralelismo_OMP.o -I$(INCLUDE) -std=c++11 

 
#Limpieza ....................................
clean: clean-objs
clean-objs:
	@echo Borrando objetos...
	-rm $(OBJ)/*
	@echo ...Borrados
	@echo
clean-bins :
	@echo Borrando ejecutables...
	-rm $(BIN)/*
	@echo ...Borrados
	@echo
clean-libs :
	@echo Borrando bibliotecas...
	-rm $(LIB)/*
	@echo ...Borrados
	@echo
mr.proper: clean-objs clean-bins clean-libs
