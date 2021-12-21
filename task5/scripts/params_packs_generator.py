lengths = range(120)
params = ["__" + str(i) + "__" for i in lengths]

for i in lengths:
    lenthesStr = ", ".join(params[:i])
    print("#define PARMS_{} {}".format(i, lenthesStr))