rm -rf build;
mkdir build;
for fileName in src/*.c; 
	do gcc ${fileName} -o build/$(basename ${fileName%.*}.o); 
done
