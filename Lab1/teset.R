k <- 10
ns <- (1:k) * 100000000
runtime <- vector(length=k)

for ( i in 1:k) {
  n <- ns[i]
  runtime[i] <- system.time(fib2(n))["user.self"]
}
plot(ns, runtime, type="b", xlab="n", ylab="runtime (second)")
grid(col="blue")