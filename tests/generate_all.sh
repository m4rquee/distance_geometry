for n in 2 3 4 5 6 7; do
  for m in "zero" "line" "full" "sqrt" "log2" "nlog2" "nsqrt"; do
    echo "generating ./grid/${n}x${n}x${n}/grid_${n}_$m.in"
    python ../scripts/grid_instance_gen.py $n $m > "./grid/${n}x${n}x${n}/grid_${n}_$m.in"
  done
done
