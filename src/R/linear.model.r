library(ggplot2)

setwd("/home/lucas/git/linefollower-arduino/")

data = read.csv("data/pwm_angle.csv")

lm(pwm ~ degree, data)
lm(degree ~ pwm, data)

pwdFromDegree <- function(degree) 27.6527 + 0.5251 * degree
degreeFromPwm <- function(pwm) -46.831 + 1.856 * pwm

data$f <- pwdFromDegree(data$degree)

ggplot(data) + geom_line(aes(degree, pwm)) + 
               geom_line(aes(degree, f))

data$g <- degreeFromPwm(data$pwm)

ggplot(data) + geom_line(aes(pwm, degree)) + 
  geom_line(aes(pwm, g))