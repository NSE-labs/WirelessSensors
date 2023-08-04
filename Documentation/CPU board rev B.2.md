# CPU Board Rev B.2
The CPU board was revised to include learnings from the Rev A.1 board.

1. The reset button placement was corrected.
2. The debug connector was changed to the standard 14-pin configuration.
3. The board was narrowed so it would fit on a standard breadboard.
4. Pin labels were added.
5. The order of the pins on the edge connectors was changed to facilitate layout on the more narrow board.

Here's the layout of the Rev B.2 board.

![Rev B.2 Layout](Images/CPU%20board%20revB.2.png)

And here's the DFM analysis from JLCPCB (board manufacturer). No errors were found.

![DFM analysis](Images/CPU%20board%20revB.2%20DFM%20Analysis.png)

The board was received and went through the same debugging process as the Rev A.1 board. Everything checked out and worked as expected. The 14-pin debug connector adds hardware reset which improves interaction with the CubeIDE debugger.

[Back to project outline](../README.md)


