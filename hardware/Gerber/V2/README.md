# Gerber V2 — ArcDice PCB revision 2

This folder contains the PCBWay-ready Gerber archive for ArcDice revision 2.

## Files

| File | Purpose |
|------|---------|
| `Archive.zip` | Complete submission package (all files below + job metadata). Send this to PCBWay. |
| `ArcDice-F_Cu.gbr` | Front copper (top layer, signal + 5V + GND pours) |
| `ArcDice-B_Cu.gbr` | Back copper (bottom layer, GND pour) |
| `ArcDice-F_Mask.gbr` | Front soldermask |
| `ArcDice-B_Mask.gbr` | Back soldermask |
| `ArcDice-F_Paste.gbr` | Front solder paste (SMD footprints; this board is through-hole so this layer is mostly empty) |
| `ArcDice-B_Paste.gbr` | Back solder paste |
| `ArcDice-F_Silkscreen.gbr` | Front silkscreen (reference designators, logo, "ArcDice V2") |
| `ArcDice-B_Silkscreen.gbr` | Back silkscreen |
| `ArcDice-Edge_Cuts.gbr` | Board outline |
| `ArcDice-PTH-drl.gbr` | Plated through-hole drill file |
| `ArcDice-NPTH-drl.gbr` | Non-plated through-hole drill file (M2 mounting holes) |
| `ArcDice-job.gbrjob` | KiCad job file describing the export — used by PCBWay to verify the package |

## Specifications

| | |
|---|---|
| **Board dimensions** | (See `ArcDice-Edge_Cuts.gbr` — open in KiCad or any Gerber viewer) |
| **Layers** | 2 (front + back) |
| **Material** | FR4 |
| **Thickness** | 1.6 mm (default; change in PCBWay order form if needed) |
| **Copper weight** | 1 oz/ft² (default) |
| **Surface finish** | HASL lead-free recommended for through-hole; ENIG also fine |
| **Soldermask colour** | Any (green is cheapest; black shows silkscreen best) |
| **Silkscreen colour** | White |
| **Min hole size** | 0.8 mm (drill diameter — through-hole pads) |
| **Min trace / space** | 0.2 mm / 0.2 mm (KiCad default; PCBWay standard) |

## Changes from V1 (if applicable)

*V1 is not in the repository — only V2 shipped publicly.*

If you ordered from a previous (pre-V2) prototype, the main changes were:

- (none — V2 is the first publicly archived revision)

## How to order

1. Go to https://www.pcbway.com/ (or any PCB house: JLCPCB, ALLPCB, etc.).
2. Click **Quick Order** → **Gerber Upload**.
3. Upload `Archive.zip`.
4. PCBWay will parse `ArcDice-job.gbrjob` and pre-fill layer count,
   material, and dimensions. Confirm:
   - **Layers**: 2
   - **Material**: FR4
   - **Thickness**: 1.6 mm
   - **Quantity**: whatever you need (5 / 10 / 30 / 50).
5. Add to cart. Choose shipping. Done.

For assembly, see [`../docs/BOM.md`](../docs/BOM.md) for the through-hole
parts. This board has no SMD components, so hand-soldering or wave
soldering both work; reflow is not required.

## Reproducing the Gerber export

If you change the schematic or PCB and want to regenerate this folder:

1. Open `../ArcDice.kicad_pro` in KiCad 8 or later.
2. **Plot** → choose **Gerber** as the format.
3. Select **all layers** you want (F.Cu, B.Cu, F.Mask, B.Mask,
   F.Paste, B.Paste, F.SilkS, B.SilkS, Edge.Cuts).
4. Check **Use Protel filename extensions** is enabled (so the
   `.gbr` filenames match).
5. Click **Plot**.
6. **Generate Drill Files** → PTH and NPTH in separate files.
   Use **Gerber X2** format.
7. Archive the generated `.gbr` + `.drl` + `*-job.gbrjob` into
   `Archive.zip`.
8. Move the previous folder to `Gerber/V3/`, update this README with
   the change log, and tag the commit as `hw-v3`.