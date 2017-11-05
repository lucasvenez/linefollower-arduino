anglesFromLineSizes <- function(a, b, c)
  acos((a**2 - c**2 - b**2)/(-2*b*c)) * 57.295779513

anglesFromLineSizes(1.25, 13.30, 13.10)
anglesFromLineSizes(1.55, 13.10, 13.00)
anglesFromLineSizes(1.55, 13.00, 13.10)
anglesFromLineSizes(1.55, 13.10, 13.30)