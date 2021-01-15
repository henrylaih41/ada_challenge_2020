make build SOURCE=../ada_challenge_2020/ada_challenge.cc 
echo "===run 00==="
./bin/ada_challenge 00 3200 < ../ada-final-public/00.in
echo "===end 00==="
echo "===run 01==="
timeout 1s ./bin/ada_challenge 01 3200 < ../ada-final-public/01.in
echo "===end 01==="
echo "===run 02==="
./bin/ada_challenge 02 3200 < ../ada-final-public/02.in
echo "===end 02==="
echo "===run 03==="
./bin/ada_challenge 03 4800 < ../ada-final-public/03.in
echo "===end 03==="
echo "===run 04==="
timeout 70s ./bin/ada_challenge 04 96000 < ../ada-final-public/04.in
echo "===end 04==="
echo "===run 05==="
timeout 1500s ./bin/ada_challenge 05 1200 < ../ada-final-public/05.in
echo "===end 05==="
echo "===run 06==="
timeout 1500s ./bin/ada_challenge 06 4800 < ../ada-final-public/06.in
echo "===end 06==="
echo "===run 07==="
timeout 1500s ./bin/ada_challenge 07 1200 < ../ada-final-public/07.in
echo "===end 07==="
echo "===run 08==="
timeout 1500s ./bin/ada_challenge 08 1200 < ../ada-final-public/08.in
echo "===end 08==="
