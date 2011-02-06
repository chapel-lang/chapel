OpenDatabase("plot.claw")

AddPlot("Pseudocolor", "col_00")

DrawPlots()

for state in range(TimeSliderGetNStates()):
    SetTimeSliderState(state)
    SaveWindow()

