###############################################
######### GENERATE MG5 MACRO TXT FILE #########
###############################################
start_CM_energy = 20  # (GeV)
end_CM_energy = 200  # (GeV)
step_CM_energy = 1  # (GeV)
steps = int(1 + (end_CM_energy - start_CM_energy) / step_CM_energy)

# Save the .txt file inside txt-commands folder
with open("macros/e-e+_e-e+.txt", "w") as file:

    #############
    ### MODEL ###
    #############
    # file.write("import model sm-lepton_masses\n")

    #############
    ## PROCESS ##
    #############
    file.write("generate e- e+ > e- e+\n")

    #############
    ## OUTPUT ###
    #############
    file.write("output simulations/e-e+_e-e+\n\n")

    for i in range(steps):
        CM_energy = start_CM_energy + i * step_CM_energy
        beam_energy = CM_energy * 0.5

        file.write("launch simulations/e-e+_e-e+\n")

        #############
        # PROGRAMS ##
        #############
        file.write("shower=OFF\n")
        file.write("detector=OFF\n")
        file.write("0\n")

        #############
        ### CARDS ###
        #############
        file.write("set nevents 1e4\n")
        # file.write(f"set ptl 0\n")
        file.write(f"set ebeam1 {beam_energy:.3f}\n")
        file.write(f"set ebeam2 {beam_energy:.3f}\n")
        file.write("0\n\n")

print("The file has been generated in the txt-commands folder.")

