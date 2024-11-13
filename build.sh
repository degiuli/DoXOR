CURRDIR=`pwd`
mkdir -pv ${CURRDIR}/bin/

# echo Building the C++ verson
cmake . -DCMAKE_BUILD_TYPE=RelWithDebInfo
make

# echo Building the Rust version
cargo build --release --verbose --bin ${CURRDIR}/target/release/DoXOR

echo Building the Java version
cd ${CURRDIR}/Java

javac -g -cp . DoXOR.java
jar cfvm DoXOR.jar manifest.txt DoXOR.class

cd ${CURRDIR}

# echo Testing C++ version...
cd ${CURRDIR}/bin/
./DoXOR
./DoXOR ${CURRDIR}/.gitignore
./DoXOR ${CURRDIR}/.gitignore 19773A56542A0424673527007E67416C073875423E72793C4E5E257B71267E14612E2F085A0037667D11767F03611048335E7825707515471F2400171E6D0531
./DoXOR ${CURRDIR}/.gitignore 19773A56542A0424673527007E67416C073875423E72793C4E5E257B71267E14
./DoXOR ${CURRDIR}/.gitignore 19773A56542A0424673527007E67416C073875423E72793C4E5E257B71267E14 AdditionalInvalidParameter

# echo Testing Rust version...
${CURRDIR}/target/release/DoXOR
${CURRDIR}/target/release/DoXOR -- ${CURRDIR}/.gitignore
${CURRDIR}/target/release/DoXOR -- ${CURRDIR}/.gitignore 19773A56542A0424673527007E67416C073875423E72793C4E5E257B71267E14612E2F085A0037667D11767F03611048335E7825707515471F2400171E6D0531
${CURRDIR}/target/release/DoXOR -- ${CURRDIR}/.gitignore 19773A56542A0424673527007E67416C073875423E72793C4E5E257B71267E14
${CURRDIR}/target/release/DoXOR -- ${CURRDIR}/.gitignore 19773A56542A0424673527007E67416C073875423E72793C4E5E257B71267E14 AdditionalInvalidParameter

echo Testing Java version...
cd ${CURRDIR}/Java

java -jar DoXOR.jar
java -jar DoXOR.jar ${CURRDIR}/.gitignore
java -jar DoXOR.jar ${CURRDIR}/.gitignore 19773A56542A0424673527007E67416C073875423E72793C4E5E257B71267E14612E2F085A0037667D11767F03611048335E7825707515471F2400171E6D0531
java -jar DoXOR.jar ${CURRDIR}/.gitignore 19773A56542A0424673527007E67416C073875423E72793C4E5E257B71267E14
java -jar DoXOR.jar ${CURRDIR}/.gitignore 19773A56542A0424673527007E67416C073875423E72793C4E5E257B71267E14 AdditionalInvalidParameter

cd ${CURRDIR}

# echo Testing Python version...
python3 ./src/DoXOR.py
python3 ./src/DoXOR.py ${CURRDIR}/.gitignore
python3 ./src/DoHMAC_MD5.py
python3 ./src/DoHMAC_MD5.py ${CURRDIR}/.gitignore