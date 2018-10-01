make
cd "C:\Program Files\qemu"
./qemu-system-arm -nographic -monitor none -serial stdio -cpu arm1176 -m 256 -M versatilepb -kernel "C:\Users\thoma\OneDrive\Documents\HR\TINBES\Kernel\kernel"

