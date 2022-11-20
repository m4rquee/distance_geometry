python ../scripts/instance_gen.py 2 zero >"./grid/2x2x2/grid_2_zero.in"
python ../scripts/instance_gen.py 2 full >"./grid/2x2x2/grid_2_full.in"

for n in 3 4 5 6 7; do
  for m in "zero" "line" "full" "sqrt" "log2" "nlog2" "nsqrt"; do
    python ../scripts/instance_gen.py $n $m >"./grid/${n}x${n}x${n}/grid_${n}_$m.in"
  done
done
