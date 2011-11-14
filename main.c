#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LENGHT 100
#define MAX_STRING 3 * MAX_LENGHT
#define MAX_LINE 100

//Tipos
typedef struct neuro {
	float weight[MAX_LENGHT];
} neuro;

typedef struct layer {
	neuro _l[MAX_LENGHT];
} layer;

//Open arquivo para leitura
void fileOpenRead(FILE **pArquivo, char *name) {
	if (!(*pArquivo = fopen(name, "r"))) {
		printf("Erro ao abrir o arquivo %s \n", name);
	}
}

//Open arquivo para escrita
void fileOpenWrite(FILE **pArquivo, char *name) {
	if (!(*pArquivo = fopen(name, "a+"))) {
		printf("Erro ao abrir o arquivo %s \n", name);
	}
}

//Write no arquivo
void writeFile(FILE **pArquivo, char *dados) {
	fprintf(*pArquivo, "%s", dados);
}

//Close no Arquivo
void closeFile(FILE **pArquivo) {
	fclose(*pArquivo);
}

//Mount arquivo CSV
void mountInput(char *stringInput, fileCSV registro) {
	int len;
	strcat(stringInput, registro.fNome);
	len = strlen(stringInput);
	stringInput[len - 1] = ',';
	//stringInput[len] = '{FONTE}';
	strcat(stringInput, registro.mNome);
	len = strlen(stringInput);
	stringInput[len - 1] = ',';
	//stringInput[len] = '{FONTE}';
	strcat(stringInput, registro.lNome);
}

//derivada da tangente hiperbôlica
double dtanh(double x) {
	return (1.0 - pow(x, 2));
}

int main(int argc, char **argv) {
	int contador = 0, auxiliar;
	char stringInput[MAX_STRING];
	char stringOutput[MAX_LINE][MAX_STRING];
	FILE *pArquivo;

	layer training_samples[MAX_LENGHT];
	float desired_output[MAX_LENGHT];

	float layers[2] = { 15, 3 };
	int number_of_entries = 3;

	float learning_rate = 0.1;
	float precision = pow(10, -6);

	int age = 0;

	float error = 0.0;

	fileOpenRead(&pArquivo);

	int contador = 0;
	while (!feof(*pArquivo)) {
		char auxiliar[MAX_LENGHT];
		fscanf(*pArquivo, "%s", auxiliar);
		contador++;
	}

CSV.open('archives/training_samples.csv', 'r', {:col_sep => ',', :converters => :float}) do |cvs|
cvs.each do |row|
@training_samples << [-1, row[0], row[1], row[2]]
@desired_output << [row[3], row[4], row[5]]
end
end

@synaptic_weights = []

@layers.each_with_index do |layer, _L|
#puts "layer: "+_L.to_s
neuro = []
if _L == 0
layer.times do |j|
#puts "neuronio: "+j.to_s
entry = []
(@number_of_entries + 1).times do |i|
#puts "entrada: "+i.to_s
entry << rand
end
neuro << entry
end
else
layer.times do |j|
#puts "neuronio: "+j.to_s
entry = []
(@layers[_L-1] + 1).times do |i|
#puts "entrada: "+i.to_s
entry << rand
end
neuro << entry
end
end
@synaptic_weights << neuro
end

@I = []
@Y = []

begin
puts "Entrando na era: "+@age.to_s
@old_error = @error
@errors = []

@training_samples.each_with_index do |ts, ti|
#------------------------------------------------------------------------------------------------ forward
# Preenchendo as matrizes I e Y
@layers.each_with_index do |layer, _L|
if _L == 0
#-------------------------------------------------- _L primeiro
#matriz I
neuro = []
layer.times do |j|
soma = 0
(@number_of_entries + 1).times do |i|
soma += @synaptic_weights[_L][j][i] * ts[i]
end
neuro << soma
end

@I << neuro

#matriz Y
neuro = []
neuro << -1
(1..layer).each do |j|
neuro << Math.tanh(@I[_L][j-1])
end

@Y << neuro
elsif _L == (@layers.size - 1)
#--------------------------------- _L último
#matriz I
neuro = []
layer.times do |j|
soma = 0
@layers[_L-1].times do |i|
soma += @synaptic_weights[_L][j][i] * @Y[_L-1][j]
end
neuro << soma
end

@I << neuro

#matriz Y
neuro = []
layer.times do |j|
neuro << Math.tanh(@I[_L][j])
end

@Y << neuro
else
#--------------------------------------------------------- _L intermediários
#matriz I
neuro = []
layer.times do |j|
soma = 0
@layers[_L-1].times do |i|
soma += @synaptic_weights[_L][j][i] * @Y[_L-1][j]
end
neuro << soma
end

@I << neuro

#matriz Y
neuro = []
neuro << -1
(1..layer).each do |j|
neuro << Math.tanh(@I[_L][j-1])
end

@Y << neuro
end
end

#------------------------------------------------------------------------------------------------- backward
@gradient = []

(@layers.size-1).downto 0 do |_L|
if _L == (@layers.size - 1)
#--------------------------------- _L último
#gradiente
neuro = []
@layers[_L].times do |j|
neuro << (@desired_output[ti][j] - @Y[_L][j]) * dtanh(@I[_L][j])
end

@gradient[_L] = neuro

#ajustando pesos sinápticos
@layers[_L].times do |j|
(@layers[_L-1] + 1).times do |i|
@synaptic_weights[_L][j][i] = @synaptic_weights[_L][j][i] + @learning_rate * @gradient[_L][j] * @Y[_L-1][i]
end
end
elsif _L == 0
#-------------------------------------------------- _L primeiro
#gradiente
neuro = []
@layers[_L].times do |j|
soma = 0
@layers[_L+1].times do |k|
soma += @gradient[_L+1][k] * @synaptic_weights[_L+1][k][j]
end
neuro << soma * dtanh(@I[_L][j])
end

@gradient[_L] = neuro

#ajustando pesos sinápticos
@layers[_L].times do |j|
(@number_of_entries + 1).times do |i|
@synaptic_weights[_L][j][i] = @synaptic_weights[_L][j][i] + @learning_rate * @gradient[_L][j] * ts[i]
end
end
else
#--------------------------------------------------------- _L intermediários
#gradiente
neuro = []
@layers[_L].times do |j|
soma = 0
@layers[_L+1].times do |k|
soma += @gradient[_L+1][k] * @synaptic_weights[_L+1][k][j]
end
neuro << soma * dtanh(@I[_L][j])
end

@gradient[_L] = neuro

#ajustando pesos sinápticos
@layers[_L].times do |j|
(@layers[_L-1] + 1).times do |i|
@synaptic_weights[_L][j][i] = @synaptic_weights[_L][j][i] + @learning_rate * @gradient[_L][j] * @Y[_L-1][i]
end
end
end
end

#------------------------------------------------------------------------------------------- Obtendo Y último atualizado
_L = @layers.size - 1
@layers.last.times do |j|
@I[_L][j] = 0
@layers[_L-1].times do |i|
@I[_L][j] += @synaptic_weights[_L][j][i] * @Y[_L-1][j]
end
end

#matriz Y
@layers.last.times do |j|
@Y[_L][j] = Math.tanh(@I[_L][j])
end

#------------------------------------------------------------------ Calcular erro local
soma = 0
@layers.last.times do |j|
soma += (@desired_output[ti][j] - @Y[@layers.size-1][j]) ** 2
end
@errors[ti] = soma / 2
end

# -------------------------------------------------------------------------------------------- Calculando erro
soma = 0
@training_samples.size.times do |k|
soma += @errors[k]
end
@error = soma / @training_samples.size

# -------------------------------------------------------------------------------------------- Contando eras
@age += 1
end until ((@error - @old_error) <= @precision) || @age > 5000

CSV.open("archives/synaptic_weights.csv", "wb") do |csv|
@layers.each_with_index do |layer, _L|
layer.times do |j|
csv << @synaptic_weights[_L][j]
end
end
end

@layers[@layers.size-1].times do |j|
puts @Y[@layers.size-1][j]
end

return 0;
}
