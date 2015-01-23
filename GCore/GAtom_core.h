#include "gcore_ptr.h"
#include "GVector_object.h"

// <html>
// <body>
// <a href="http://en.wikipedia.org/wiki/Glycine">			http://en.wikipedia.org/wiki/Glycine		</a>		
// <a href="http://en.wikipedia.org/wiki/Alanine">			http://en.wikipedia.org/wiki/Alanine		</a>		
// <a href="http://en.wikipedia.org/wiki/Serine">			http://en.wikipedia.org/wiki/Serine			</a>		
// <a href="http://en.wikipedia.org/wiki/Threonine">		http://en.wikipedia.org/wiki/Threonine		</a>		
// <a href="http://en.wikipedia.org/wiki/Cysteine">			http://en.wikipedia.org/wiki/Cysteine		</a>		
// <a href="http://en.wikipedia.org/wiki/Valine">			http://en.wikipedia.org/wiki/Valine			</a>		
// <a href="http://en.wikipedia.org/wiki/Leucine">			http://en.wikipedia.org/wiki/Leucine		</a>		
// <a href="http://en.wikipedia.org/wiki/Isoleucine">		http://en.wikipedia.org/wiki/Isoleucine		</a>	
// <a href="http://en.wikipedia.org/wiki/Methionine">		http://en.wikipedia.org/wiki/Methionine		</a>	
// <a href="http://en.wikipedia.org/wiki/Proline">			http://en.wikipedia.org/wiki/Proline		</a>		
// <a href="http://en.wikipedia.org/wiki/Phenylalanine">	http://en.wikipedia.org/wiki/Phenylalanine	</a>	
// <a href="http://en.wikipedia.org/wiki/Tyrosine">			http://en.wikipedia.org/wiki/Tyrosine		</a>		
// <a href="http://en.wikipedia.org/wiki/Tryptophan">		http://en.wikipedia.org/wiki/Tryptophan		</a>	
// <a href="http://en.wikipedia.org/wiki/Aspartic_acid">	http://en.wikipedia.org/wiki/Aspartic_acid	</a>	
// <a href="http://en.wikipedia.org/wiki/Glutamic_acid">	http://en.wikipedia.org/wiki/Glutamic_acid	</a>	
// <a href="http://en.wikipedia.org/wiki/Asparagine">		http://en.wikipedia.org/wiki/Asparagine		</a>	
// <a href="http://en.wikipedia.org/wiki/Glutamine">		http://en.wikipedia.org/wiki/Glutamine		</a>		
// <a href="http://en.wikipedia.org/wiki/Histidine">		http://en.wikipedia.org/wiki/Histidine		</a>		
// <a href="http://en.wikipedia.org/wiki/Lysine">			http://en.wikipedia.org/wiki/Lysine			</a>		
// <a href="http://en.wikipedia.org/wiki/Arginine">			http://en.wikipedia.org/wiki/Arginine		</a>		
// </body>
// </html>

#ifndef _GATOM_CORE_H
#define _GATOM_CORE_H

namespace god
{
#pragma pack(push, 1)
	typedef uint8_t atom_t;

	static const atom_t	ATOM_VOID				= 0;
	static const atom_t	ATOM_HYDROGEN			= 1;
	static const atom_t	ATOM_HELIUM				= 2;
	static const atom_t	ATOM_LITHIUM			= 3;
	static const atom_t	ATOM_BERYLLIUM			= 4;
	static const atom_t	ATOM_BORON				= 5;
	static const atom_t	ATOM_CARBON				= 6;
	static const atom_t	ATOM_NITROGEN			= 7;
	static const atom_t	ATOM_OXYGEN				= 8;
	static const atom_t	ATOM_FLUORINE			= 9;
	static const atom_t	ATOM_NEON				= 10;
	static const atom_t	ATOM_SODIUM				= 11;
	static const atom_t	ATOM_MAGNESIUM			= 12;
	static const atom_t	ATOM_ALUMINIUM			= 13;
	static const atom_t	ATOM_SILICON			= 14;
	static const atom_t	ATOM_PHOSPHORUS			= 15;
	static const atom_t	ATOM_SULFUR				= 16;
	static const atom_t	ATOM_CHLORINE			= 17;
	static const atom_t	ATOM_ARGON				= 18;
	static const atom_t	ATOM_POTASSIUM			= 19;
	static const atom_t	ATOM_CALCIUM			= 20;
	static const atom_t	ATOM_SCANDIUM			= 21;
	static const atom_t	ATOM_TITANIUM			= 22;
	static const atom_t	ATOM_VANADIUM			= 23;
	static const atom_t	ATOM_CHROMIUM			= 24;
	static const atom_t	ATOM_MANGANESE			= 25;
	static const atom_t	ATOM_IRON				= 26;
	static const atom_t	ATOM_COBALT				= 27;
	static const atom_t	ATOM_NICKEL				= 28;
	static const atom_t	ATOM_COPPER				= 29;
	static const atom_t	ATOM_ZINC				= 30;
	static const atom_t	ATOM_GALLIUM			= 31;
	static const atom_t	ATOM_GERMANIUM			= 32;
	static const atom_t	ATOM_ARSENIC			= 33;
	static const atom_t	ATOM_SELENIUM			= 34;
	static const atom_t	ATOM_BROMINE			= 35;
	static const atom_t	ATOM_KRYPTON			= 36;
	static const atom_t	ATOM_RUBIDIUM			= 37;
	static const atom_t	ATOM_STRONTIUM			= 38;
	static const atom_t	ATOM_YTTRIUM			= 39;
	static const atom_t	ATOM_ZIRCONIUM			= 40;
	static const atom_t	ATOM_NIOBIUM			= 41;
	static const atom_t	ATOM_MOLYBDENUM			= 42;
	static const atom_t	ATOM_TECHNETIUM			= 43;
	static const atom_t	ATOM_RUTHENIUM			= 44;
	static const atom_t	ATOM_RHODIUM			= 45;
	static const atom_t	ATOM_PALLADIUM			= 46;
	static const atom_t	ATOM_SILVER				= 47;
	static const atom_t	ATOM_CADMIUM			= 48;
	static const atom_t	ATOM_INDIUM				= 49;
	static const atom_t	ATOM_TIN				= 50;
	static const atom_t	ATOM_ANTIMONY			= 51;
	static const atom_t	ATOM_TELLURIUM			= 52;
	static const atom_t	ATOM_IODINE				= 53;
	static const atom_t	ATOM_XENON				= 54;
	static const atom_t	ATOM_CAESIUM			= 55;
	static const atom_t	ATOM_BARIUM				= 56;
	static const atom_t	ATOM_LANTHANUM			= 57;
	static const atom_t	ATOM_CERIUM				= 58;
	static const atom_t	ATOM_PRASEODYMIUM		= 59;
	static const atom_t	ATOM_NEODYMIUM			= 60;
	static const atom_t	ATOM_PROMETHIUM			= 61;
	static const atom_t	ATOM_SAMARIUM			= 62;
	static const atom_t	ATOM_EUROPIUM			= 63;
	static const atom_t	ATOM_GADOLINIUM			= 64;
	static const atom_t	ATOM_TERBIUM			= 65;
	static const atom_t	ATOM_DYSPROSIUM			= 66;
	static const atom_t	ATOM_HOLMIUM			= 67;
	static const atom_t	ATOM_ERBIUM				= 68;
	static const atom_t	ATOM_THULIUM			= 69;
	static const atom_t	ATOM_YTTERBIUM			= 70;
	static const atom_t	ATOM_LUTETIUM			= 71;
	static const atom_t	ATOM_HAFNIUM			= 72;
	static const atom_t	ATOM_TANTALUM			= 73;
	static const atom_t	ATOM_TUNGSTEN			= 74;
	static const atom_t	ATOM_RHENIUM			= 75;
	static const atom_t	ATOM_OSMIUM				= 76;
	static const atom_t	ATOM_IRIDIUM			= 77;
	static const atom_t	ATOM_PLATINUM			= 78;
	static const atom_t	ATOM_GOLD				= 79;
	static const atom_t	ATOM_MERCURY			= 80;
	static const atom_t	ATOM_THALLIUM			= 81;
	static const atom_t	ATOM_LEAD				= 82;
	static const atom_t	ATOM_BISMUTH			= 83;
	static const atom_t	ATOM_POLONIUM			= 84;
	static const atom_t	ATOM_ASTATINE			= 85;
	static const atom_t	ATOM_RADON				= 86;
	static const atom_t	ATOM_FRANCIUM			= 87;
	static const atom_t	ATOM_RADIUM				= 88;
	static const atom_t	ATOM_ACTINIUM			= 89;
	static const atom_t	ATOM_THORIUM			= 90;
	static const atom_t	ATOM_PROTACTINIUM		= 91;
	static const atom_t	ATOM_URANIUM			= 92;
	static const atom_t	ATOM_NEPTUNIUM			= 93;
	static const atom_t	ATOM_PLUTONIUM			= 94;
	static const atom_t	ATOM_AMERICIUM			= 95;
	static const atom_t	ATOM_CURIUM				= 96;
	static const atom_t	ATOM_BERKELIUM			= 97;
	static const atom_t	ATOM_CALIFORNIUM		= 98;
	static const atom_t	ATOM_EINSTEINIUM		= 99;
	static const atom_t	ATOM_FERMIUM			= 100;
	static const atom_t	ATOM_MENDELEVIUM		= 101;
	static const atom_t	ATOM_NOBELIUM			= 102;
	static const atom_t	ATOM_LAWRENCIUM			= 103;
	static const atom_t	ATOM_RUTHERFORDIUM		= 104;
	static const atom_t	ATOM_DUBNIUM			= 105;
	static const atom_t	ATOM_SEABORGIUM			= 106;
	static const atom_t	ATOM_BOHRIUM			= 107;
	static const atom_t	ATOM_HASSIUM			= 108;
	static const atom_t	ATOM_MEITNERIUM 		= 109;
	static const atom_t	ATOM_DARMSTADTIUM		= 110;
	static const atom_t	ATOM_ROENTGENIUM		= 111;
	static const atom_t	ATOM_COPERNICIUM		= 112;
	static const atom_t	ATOM_UNUNTRIUM			= 113;
	static const atom_t	ATOM_FLEROVIUM			= 114;
	static const atom_t	ATOM_UNUNPENTIUM		= 115;
	static const atom_t	ATOM_LIVERMORIUM		= 116;
	static const atom_t	ATOM_UNUNSEPTIUM		= 117;
	static const atom_t	ATOM_UNUNOCTIUM			= 118;

	GDECLARE_CORE_STRUCT( ATOM, Atom, 
		atom_t		AtomicNumber;		//
		char		Symbol		[4];	//
		uint32_t	ColorSolid;			//
		uint32_t	ColorLiquid;		//
		uint32_t	ColorGas;			//
		uint32_t	ColorCrystal;		//
		uint32_t	ColorElectricMax;	//
		uint32_t	ColorElectricMin;	//
		char		Name		[64];	//
		char		NameOrigin	[256];	//
		uint8_t		Group;				//
		uint8_t		Period;				//
		double		AtomicWeight;		// u ();
		double		Density;			// g / cm3;
		double		MeltPoint;			// Kelvin
		double		BoilPoint;			// Kelvin
		double		Heat;				//  J/g·K;
		double		Electronegativity;	// Neg^10
		double		Abundance;			//
	);

	static void onCreateAtomDefault(ggns_Atom* stuff)
	{
		memset((*stuff).get_data_address(), 0, (*stuff).get_data_size());
	}

	typedef uint8_t GFINTERACTION_TYPE;
	static const GFINTERACTION_TYPE GFINTERACTIONTYPE_STRONG			= 1;
	static const GFINTERACTION_TYPE GFINTERACTIONTYPE_WEAK				= 2;
	static const GFINTERACTION_TYPE GFINTERACTIONTYPE_ELECTROMAGNETIC	= 3;
	static const GFINTERACTION_TYPE GFINTERACTIONTYPE_GRAVITATIONAL		= 4;

	GDECLARE_CORE_STRUCT( PARTICLE_STATE, ParticleState,
		uint8_t		AtomType;
		uint8_t		Spin;
		uint16_t	Energy;
	);
	static void onCreateParticleStateDefault(ggns_ParticleState* stuff)
	{
		memset((*stuff).get_data_address(), 0, (*stuff).get_data_size());
	};

	GDECLARE_CORE_STRUCT( ATOM_STATES, AtomStates, 
		uint16_t	ElectronCount;
		uint16_t	ProtonCount;
		uint16_t	NeutronCount;
		uint16_t	Energy;
	);
	static void onCreateAtomStatesDefault(ggns_AtomStates* stuff)
	{
		memset((*stuff).get_data_address(), 0, (*stuff).get_data_size());
	};

	GDECLARE_CORE_STRUCT( PHOTON_STATES, PhotonStates, 
		float		Wavelength;
		GVector3	Direction;
		uint8_t		Polarization;
	);
	static void onCreatePhotonStatesDefault(ggns_PhotonStates* stuff)
	{
		memset((*stuff).get_data_address(), 0, (*stuff).get_data_size());
	};

	static const SAtom g_PeriodicTable[256] = {
		//-------------------------------------------------------------------------
		// N	// Sym	//ColorSolid	// ColorLiquid	// ColorGas		// ColorCrystal	// ColorElectricMax	// ColorElectricMin	// Name				// NameOrigin																																				// Group	// Period	// AtomicWeight	// Density	// MeltPoint	// BoilPoint	// Heat		// Electronegativity	// Abundance
		{ 0,	"Vd",	0xFF000000,		0xFF000000,		0xFF000000,		0xFF000000,		0xFF000000,			0xFF000000,			"Void",				"From a dialect variant of Old French vuide; related to Latin vacare ‘vacate’; the verb partly a shortening of avoid, reinforced by Old French voider.",	0,			0,			0,				0,			0,				0,				0,			0,						0			},
		{ 1,	"H",	0x10FFFFFF,		0x10FFFFFF,		0x10FFFFFF,		0x10FFFFFF,		0xFFFF40FF,			0xFFFF40FF,			"Hydrogen",			"the Greek 'hydro' and 'genes' meaning water-forming",																										1,			1,			1.0082,			0.00008988, 14.01,			20.28,			14.304,		2.20,					1400		},
		{ 2,	"He",	0xFF000000,		0xFF000000,		0xFF000000,		0xFFFFFFFF,		0xFFFFFFFF,			0xFFC03600,			"Helium",			"the Greek 'helios' meaning sun",																															18,			1,			4.002602,		0.0001785,	0.956,			4.22,			5.193,		-1,						0.008		},
		//-------------------------------------------------------------------------
		// N	// Sym	//ColorSolid	// ColorLiquid	// ColorGas		// ColorCrystal	// ColorElectricMax	// ColorElectricMin	// Name				// NameOrigin																																				// Group	// Period	// AtomicWeight	// Density	// MeltPoint	// BoilPoint	// Heat		// Electronegativity	// Abundance
		{ 3,	"Li",	0xFFC0C0C0,		0xFFF0F0F0,		0xFFFFFFFF,		0xFFFFFFFF,		0xFFFFFFFF,			0xFFFFFFFF,			"Lithium",			"the Greek 'lithos' meaning stone",																															1,			2,			6.942,			0.534,		453.69,			1560,			3.582,		0.98,					20			},
		{ 4,	"Be",	0xFF000000,		0xFF808080,		0xFF808080,		0xFF808080,		0xFFFF0000,			0xFF808080,			"Beryllium",		"the Greek name for beryl, 'beryllo'",																														2,			2,			9.012182,		1.85,		1560,			2742,			1.825,		1.57,					2.8			},
		{ 5,	"B",	0xFF000000,		0xFFFF0000,		0xFFFF0000,		0xFFFFFFFF,		0xFFFFFFFF,			0xFFFF0000,			"Boron",			"the Arabic 'buraq', which was the name for borax",																											13,			2,			10.812,			2.34,		2349,			4200,			1.026,		2.04,					10			},
		{ 6,	"C",	0xFF000000,		0xFF000000,		0xFF000000,		0x10FFFFFF,		0xFF000000,			0xFF000000,			"Carbon",			"the Latin 'carbo', meaning charcoal",																														14,			2,			12.0112,		2.267,		3800,			4300,			0.709,		2.55,					200			},
		{ 7,	"N",	0x10FFFFFF,		0x10FFFFFF,		0x10FFFFFF,		0x10FFFFFF,		0xFFFFFFFF,			0xFFFFFFFF,			"Nitrogen",			"the Greek 'nitron' and 'genes' meaning nitre-forming",																										15,			2,			14.0072,		0.0012506,	63.15,			77.36,			1.04,		3.04,					19			},
		{ 8,	"O",	0x10FFFFFF,		0xFFC0C0FF,		0x10FFFFFF,		0xFFC0C0FF,		0xFFC0C0FF,			0xFFC0C0FF,			"Oxygen",			"the Greek 'oxy' and 'genes' meaning acid-forming",																											16,			2,			15.9992,		0.001429,	54.36,			90.20,			0.918,		3.44,					461000		},
		{ 9,	"F",	0xFF000000,		0xFFC09E00,		0xFFC09E00,		0xFFC09E00,		0xFF000000,			0xFF000000,			"Fluorine",			"the Latin 'fluere', meaning to flow",																														17,			2,			18.9984032,		0.001696,	53.53,			85.03,			0.824,		3.98,					585			},
		{ 10,	"Ne",	0x10FFFFFF,		0x10FFFFFF,		0x10FFFFFF,		0x10FFFFFF,		0xFFFFC600,			0xFFFF0000,			"Neon",				"the Greek 'neos', meaning new",																															18,			2,			20.1797,		0.0008999,	24.56,			27.07,			1.03,		-1,						0.005		},
		//-------------------------------------------------------------------------
		// N	// Sym	//ColorSolid	// ColorLiquid	// ColorGas		// ColorCrystal	// ColorElectricMax	// ColorElectricMin	// Name				// NameOrigin																																				// Group	// Period	// AtomicWeight	// Density	// MeltPoint	// BoilPoint	// Heat		// Electronegativity	// Abundance
		{ 11,	"Na",	0xFF000000,		0xFF000000,		0xFF000000,		0xFF000000,		0xFF000000,			0xFF000000,			"Sodium",			"the English word soda (natrium in Latin)[2]",																												1,			3,			22.98976928,	0.971,		370.87,			1156,			1.228,		0.93,					23600		},
		{ 12,	"Mg",	0xFF000000,		0xFF000000,		0xFF000000,		0xFF000000,		0xFF000000,			0xFF000000,			"Magnesium",		"Magnesia, a district of Eastern Thessaly in Greece",																										2,			3,			24.3059,		1.738,		923,			1363,			1.023,		1.31,					23300		},
		{ 13,	"Al",	0xFF000000,		0xFF000000,		0xFF000000,		0xFF000000,		0xFF000000,			0xFF000000,			"Aluminium",		"the Latin name for alum, 'alumen' meaning bitter salt",																									13,			3,			26.9815386,		2.698,		933.47,			2792,			0.897,		1.61,					82300		},
		{ 14,	"Si",	0xFF000000,		0xFF000000,		0xFF000000,		0xFF000000,		0xFF000000,			0xFF000000,			"Silicon",			"the Latin 'silex' or 'silicis', meaning flint",																											14,			3,			28.0854,		2.3296,		1687,			3538,			0.705,		1.9,					282000		},
		{ 15,	"P",	0xFFDD0000,		0xFFDD0000,		0xFFDD0000,		0xFFDD0000,		0xFFDD0000,			0xFF000000,			"Phosphorus",		"the Greek 'phosphoros', meaning bringer of light",																											15,			3,			30.973762,		1.82,		317.30,			550,			0.769,		2.19,					1050		},
		{ 16,	"S",	0xFFFFF000,		0xFFFFF000,		0xFFFFF000,		0xFFFFF000,		0xFFFFF000,			0xFFFFF000,			"Sulfur",			"Either from the Sanskrit 'sulvere', or the Latin 'sulfurium', both names for sulfur[2]",																	16,			3,			32.062,			2.067,		388.36,			717.87,			0.71,		2.58,					350			},
		{ 17,	"Cl",	0xFF000000,		0xFF000000,		0xFF000000,		0xFF000000,		0xFF000000,			0xFF000000,			"Chlorine",			"the Greek 'chloros', meaning greenish yellow",																												17,			3,			35.452,			0.003214,	171.6,			239.11,			0.479,		3.16,					145			},
		{ 18,	"Ar",	0xFF000000,		0xFF000000,		0xFF000000,		0xFF000000,		0xFF000000,			0xFF000000,			"Argon",			"the Greek, 'argos', meaning idle",																															18,			3,			39.948,			0.0017837,	83.80,			87.30,			0.52,		-1,						3.5			},
		//-------------------------------------------------------------------------
		// N	// Sym	//ColorSolid	// ColorLiquid	// ColorGas		// ColorCrystal	// ColorElectricMax	// ColorElectricMin	// Name				// NameOrigin																																				// Group	// Period	// AtomicWeight	// Density	// MeltPoint	// BoilPoint	// Heat		// Electronegativity	// Abundance
		{ 19,	"K", 	0xFF000000,		0xFF000000,		0xFF000000,		0xFF000000,		0xFF000000,			0xFF000000,			"Potassium",		"the English word potash (kalium in Latin)[2]",																												1,			4,			39.0983,		0.862,		336.53,			1032,			0.757,		0.82,					20900		},
		{ 20,	"Ca",	0xFF000000,		0xFF000000,		0xFF000000,		0xFF000000,		0xFF000000,			0xFF000000,			"Calcium",			"the Latin 'calx' meaning lime",																															2,			4,			40.078,			1.54,		1115,			1757,			0.647,		1,						41500		},
		{ 21,	"Sc",	0xFF000000,		0xFF000000,		0xFF000000,		0xFF000000,		0xFF000000,			0xFF000000,			"Scandium",			"Scandinavia (with the Latin name Scandia)",																												3,			4,			44.955912,		2.989,		1814,			3109,			0.568,		1.36,					22			},
		{ 22,	"Ti",	0xFF000000,		0xFF000000,		0xFF000000,		0xFF000000,		0xFF000000,			0xFF000000,			"Titanium",			"Titans, the sons of the Earth goddess of Greek mythology",																									4,			4,			47.867,			4.54,		1941,			3560,			0.523,		1.54,					5650		},
		{ 23,	"V", 	0xFF000000,		0xFF000000,		0xFF000000,		0xFF000000,		0xFF000000,			0xFF000000,			"Vanadium",			"Vanadis, an old Norse name for the Scandinavian goddess Freyja",																							5,			4,			50.9415,		6.11,		2183,			3680,			0.489,		1.63,					120			},
		{ 24,	"Cr",	0xFF000000,		0xFF000000,		0xFF000000,		0xFF000000,		0xFF000000,			0xFF000000,			"Chromium",			"the Greek 'chroma', meaning colour",																														6,			4,			51.9961,		7.15,		2180,			2944,			0.449,		1.66,					102			},
		{ 25,	"Mn",	0xFF000000,		0xFF000000,		0xFF000000,		0xFF000000,		0xFF000000,			0xFF000000,			"Manganese",		"Either the Latin 'magnes', meaning magnet or from the black magnesium oxide, 'magnesia nigra'",															7,			4,			54.938045,		7.44,		1519,			2334,			0.479,		1.55,					950			},
		{ 26,	"Fe",	0xFF000000,		0xFF000000,		0xFF000000,		0xFF000000,		0xFF000000,			0xFF000000,			"Iron",				"the Anglo-Saxon name iren (ferrum in Latin)",																												8,			4,			55.845,			7.874,		1811,			3134,			0.449,		1.83,					56300		},
		{ 27,	"Co",	0xFF000000,		0xFF000000,		0xFF000000,		0xFF000000,		0xFF000000,			0xFF000000,			"Cobalt",			"the German word 'kobald', meaning goblin",																													9,			4,			58.933195,		8.86,		1768,			3200,			0.421,		1.88,					25			},
		{ 28,	"Ni",	0xFF000000,		0xFF000000,		0xFF000000,		0xFF000000,		0xFF000000,			0xFF000000,			"Nickel",			"the shortened of the German 'kupfernickel' meaning either devil's copper or St. Nicholas's copper",														10,			4,			58.6934,		8.912,		1728,			3186,			0.444,		1.91,					84			},
		{ 29,	"Cu",	0xFF000000,		0xFF000000,		0xFF000000,		0xFF000000,		0xFF000000,			0xFF000000,			"Copper",			"the Old English name coper in turn derived from the Latin 'Cyprium aes', meaning a metal from Cyprus",														11,			4,			63.546,			8.96,		1357.77,		2835,			0.385,		1.9,					60			},
		{ 30,	"Zn",	0xFF000000,		0xFF000000,		0xFF000000,		0xFF000000,		0xFF000000,			0xFF000000,			"Zinc",				"the German, 'zinc', which may in turn be derived from the Persian word 'sing', meaning stone",																12,			4,			65.38,			7.134,		692.88,			1180,			0.388,		1.65,					70			},
		{ 31,	"Ga",	0xFF000000,		0xFF000000,		0xFF000000,		0xFF000000,		0xFF000000,			0xFF000000,			"Gallium",			"France (with the Latin name Gallia)",																														13,			4,			69.723,			5.907,		302.9146,		2477,			0.371,		1.81,					19			},
		{ 32,	"Ge",	0xFF000000,		0xFF000000,		0xFF000000,		0xFF000000,		0xFF000000,			0xFF000000,			"Germanium",		"Germany (with the Latin name Germania)",																													14,			4,			72.630,			5.323,		1211.40,		3106,			0.32,		2.01,					1.5			},
		{ 33,	"As",	0xFF000000,		0xFF000000,		0xFF000000,		0xFF000000,		0xFF000000,			0xFF000000,			"Arsenic",			"the Greek name 'arsenikon' for the yellow pigment orpiment",																								15,			4,			74.92160,		5.776,		1090,			887,			0.329,		2.18,					1.8			},
		{ 34,	"Se",	0xFFC0C0C0,		0xFFFF0000,		0xFFFF0000,		0xFFFF0000,		0xFFFF0000,			0xFFFF0000,			"Selenium",			"Moon (with the Greek name selene)",																														16,			4,			78.96,			4.809,		453,			958,			0.321,		2.55,					0.05		},
		{ 35,	"Br",	0xFF000000,		0xFF000000,		0xFF000000,		0xFF000000,		0xFF000000,			0xFF000000,			"Bromine",			"the Greek 'bromos' meaning stench",																														17,			4,			79.9049,		3.122,		265.8,			332.0,			0.474,		2.96,					2.4			},
		{ 36,	"Kr",	0xFF000000,		0xFF000000,		0xFF000000,		0xFF000000,		0xFF000000,			0xFF000000,			"Krypton",			"the Greek 'kryptos', meaning hidden",																														18,			4,			83.798,			0.003733,	115.79,			119.93,			0.248,		3,						0.0001		},
		//-------------------------------------------------------------------------
		// N	// Sym	//ColorSolid	// ColorLiquid	// ColorGas		// ColorCrystal	// ColorElectricMax	// ColorElectricMin	// Name				// NameOrigin																																				// Group	// Period	// AtomicWeight	// Density	// MeltPoint	// BoilPoint	// Heat		// Electronegativity	// Abundance
		{ 37,	"Rb",	0xFF000000,		0xFF000000,		0xFF000000,		0xFF000000,		0xFF000000,			0xFF000000,			"Rubidium",			"the Latin 'rubidius', meaning deepest red",																												1,			5,			85.4678,		1.532,		312.46,			961,			0.363,		0.82,					90			},
		{ 38,	"Sr",	0xFF000000,		0xFF000000,		0xFF000000,		0xFF000000,		0xFF000000,			0xFF000000,			"Strontium",		"Strontian, a small town in Scotland",																														2,			5,			87.62,			2.64,		1050,			1655,			0.301,		0.95,					370			},
		{ 39,	"Y",	0xFF000000,		0xFF000000,		0xFF000000,		0xFF000000,		0xFF000000,			0xFF000000,			"Yttrium",			"Ytterby, Sweden",																																			3,			5,			88.90585,		4.469,		1799,			3609,			0.298,		1.22,					33			},
		{ 40,	"Zr",	0xFF000000,		0xFF000000,		0xFF000000,		0xFF000000,		0xFF000000,			0xFF000000,			"Zirconium",		"the Persian 'zargun', meaning gold coloured",																												4,			5,			91.224,			6.506,		2128,			4682,			0.278,		1.33,					165			},
		{ 41,	"Nb",	0xFF000000,		0xFF000000,		0xFF000000,		0xFF000000,		0xFF000000,			0xFF000000,			"Niobium",			"Niobe, daughter of king Tantalus from Greek mythology",																									5,			5,			92.90638,		8.57,		2750,			5017,			0.265,		1.6,					20			},
		{ 42,	"Mo",	0xFF000000,		0xFF000000,		0xFF000000,		0xFF000000,		0xFF000000,			0xFF000000,			"Molybdenum",		"the Greek 'molybdos' meaning lead",																														6,			5,			95.96,			10.22,		2896,			4912,			0.251,		2.16,					1.2			},
		{ 43,	"Tc",	0xFF000000,		0xFF000000,		0xFF000000,		0xFF000000,		0xFF000000,			0xFF000000,			"Technetium",		"the Greek 'tekhnetos' meaning artificial",																													7,			5,			98,				11.5,		2430,			4538,			-1,			1.9,					0.0001		},
		{ 44,	"Ru",	0xFF000000,		0xFF000000,		0xFF000000,		0xFF000000,		0xFF000000,			0xFF000000,			"Ruthenium",		"Russia (with the Latin name Ruthenia)",																													8,			5,			101.07,			12.37,		2607,			4423,			0.238,		2.2,					0.001		},
		{ 45,	"Rh",	0xFF000000,		0xFF000000,		0xFF000000,		0xFF000000,		0xFF000000,			0xFF000000,			"Rhodium",			"the Greek 'rhodon', meaning rose coloured",																												9,			5,			102.90550,		12.41,		2237,			3968,			0.243,		2.28,					0.001		},
		{ 46,	"Pd",	0xFF000000,		0xFF000000,		0xFF000000,		0xFF000000,		0xFF000000,			0xFF000000,			"Palladium",		"the then recently discovered asteroid Pallas, considered a planet at the time",																			10,			5,			106.42,			12.02,		1828.05,		3236,			0.244,		2.2,					0.015		},
		{ 47,	"Ag",	0xFF000000,		0xFF000000,		0xFF000000,		0xFF000000,		0xFF000000,			0xFF000000,			"Silver",			"the Anglo-Saxon name siolfur (argentum in Latin)[2]",																										11,			5,			107.8682,		10.501,		1234.93,		2435,			0.235,		1.93,					0.075		},
		{ 48,	"Cd",	0xFF000000,		0xFF000000,		0xFF000000,		0xFF000000,		0xFF000000,			0xFF000000,			"Cadmium",			"the Latin name for the mineral calmine, 'cadmia'",																											12,			5,			112.411,		8.69,		594.22,			1040,			0.232,		1.69,					0.159		},
		{ 49,	"In",	0xFF000000,		0xFF000000,		0xFF000000,		0xFF000000,		0xFF000000,			0xFF000000,			"Indium",			"the Latin 'indicium', meaning violet or indigo",																											13,			5,			114.818,		7.31,		429.75,			2345,			0.233,		1.78,					0.25		},
		{ 50,	"Sn",	0xFF000000,		0xFF000000,		0xFF000000,		0xFF000000,		0xFF000000,			0xFF000000,			"Tin",				"the Anglo-Saxon word tin (stannum in Latin, meaning hard)",																								14,			5,			118.710,		7.287,		505.08,			2875,			0.228,		1.96,					2.3			},
		{ 51,	"Sb",	0xFF000000,		0xFF000000,		0xFF000000,		0xFF000000,		0xFF000000,			0xFF000000,			"Antimony",			"the Greek 'anti -1 monos', meaning not alone (stibium in Latin)",																							15,			5,			121.760,		6.685,		903.78,			1860,			0.207,		2.05,					0.2			},
		{ 52,	"Te",	0xFF000000,		0xFF000000,		0xFF000000,		0xFF000000,		0xFF000000,			0xFF000000,			"Tellurium",		"Earth, the third planet on solar system (with the Latin word tellus)",																						16,			5,			127.60,			6.232,		722.66,			1261,			0.202,		2.1,					0.001		},
		{ 53,	"I",	0xFF000000,		0xFF000000,		0xFF000000,		0xFF000000,		0xFF000000,			0xFF000000,			"Iodine",			"the Greek 'iodes' meaning violet",																															17,			5,			126.90447,		4.93,		386.85,			457.4,			0.214,		2.66,					0.45		},
		{ 54,	"Xe",	0xFF000000,		0xFF000000,		0xFF000000,		0xFF000000,		0xFF000000,			0xFF000000,			"Xenon",			"the Greek 'xenos' meaning stranger",																														18,			5,			131.293,		0.005887,	161.4,			165.03,			0.158,		2.6,					0.0001		},
		//-------------------------------------------------------------------------
		// N	// Sym	//ColorSolid	// ColorLiquid	// ColorGas		// ColorCrystal	// ColorElectricMax	// ColorElectricMin	// Name				// NameOrigin																																				// Group	// Period	// AtomicWeight	// Density	// MeltPoint	// BoilPoint	// Heat		// Electronegativity	// Abundance
		{ 55,	"Cs",	0xFF000000,		0xFF000000,		0xFF000000,		0xFF000000,		0xFF000000,			0xFF000000,			"Caesium",			"the Latin 'caesius', meaning sky blue",																													1,			6,			132.9054519,	1.873,		301.59,			944,			0.242,		0.79,					3			},
		{ 56,	"Ba",	0xFF000000,		0xFF000000,		0xFF000000,		0xFF000000,		0xFF000000,			0xFF000000,			"Barium",			"the Greek 'barys', meaning heavy",																															2,			6,			137.327,		3.594,		1000,			2170,			0.204,		0.89,					425			},
		{ 57,	"La",	0xFF000000,		0xFF000000,		0xFF000000,		0xFF000000,		0xFF000000,			0xFF000000,			"Lanthanum",		"the Greek 'lanthanein', meaning to lie hidden",																											0,			6,			138.90547,		6.145,		1193,			3737,			0.195,		1.1,					39			},
		{ 58,	"Ce",	0xFF000000,		0xFF000000,		0xFF000000,		0xFF000000,		0xFF000000,			0xFF000000,			"Cerium",			"Ceres, the Roman God of agriculture",																														0,			6,			140.116,		6.77,		1068,			3716,			0.192,		1.12,					66.5		},
		{ 59,	"Pr",	0xFF000000,		0xFF000000,		0xFF000000,		0xFF000000,		0xFF000000,			0xFF000000,			"Praseodymium",		"the Greek 'prasios didymos' meaning green twin",																											0,			6,			140.90765,		6.773,		1208,			3793,			0.193,		1.13,					9.2			},
		{ 60,	"Nd",	0xFF000000,		0xFF000000,		0xFF000000,		0xFF000000,		0xFF000000,			0xFF000000,			"Neodymium",		"the Greek 'neos didymos' meaning new twin",																												0,			6,			144.242,		7.007,		1297,			3347,			0.19,		1.14,					41.5		},
		{ 61,	"Pm",	0xFF000000,		0xFF000000,		0xFF000000,		0xFF000000,		0xFF000000,			0xFF000000,			"Promethium",		"Prometheus of Greek mythology who stole fire from the Gods and gave it to humans",																			0,			6,			145,			7.26,		1315,			3273,			-1,			1.13,					0.0001		},
		{ 62,	"Sm",	0xFF000000,		0xFF000000,		0xFF000000,		0xFF000000,		0xFF000000,			0xFF000000,			"Samarium",			"Samarskite, the name of the mineral from which it was first isolated",																						0,			6,			150.36,			7.52,		1345,			2067,			0.197,		1.17,					7.05		},
		{ 63,	"Eu",	0xFF000000,		0xFF000000,		0xFF000000,		0xFF000000,		0xFF000000,			0xFF000000,			"Europium",			"Europe",																																					0,			6,			151.964,		5.243,		1099,			1802,			0.182,		1.2,					2			},
		{ 64,	"Gd",	0xFF000000,		0xFF000000,		0xFF000000,		0xFF000000,		0xFF000000,			0xFF000000,			"Gadolinium",		"Johan Gadolin, chemist, physicist and mineralogist",																										0,			6,			157.25,			7.895,		1585,			3546,			0.236,		1.2,					6.2			},
		{ 65,	"Tb",	0xFF000000,		0xFF000000,		0xFF000000,		0xFF000000,		0xFF000000,			0xFF000000,			"Terbium",			"Ytterby, Sweden",																																			0,			6,			158.92535,		8.229,		1629,			3503,			0.182,		1.2,					1.2			},
		{ 66,	"Dy",	0xFF000000,		0xFF000000,		0xFF000000,		0xFF000000,		0xFF000000,			0xFF000000,			"Dysprosium",		"the Greek 'dysprositos', meaning hard to get",																												0,			6,			162.500,		8.55,		1680,			2840,			0.17,		1.22,					5.2			},
		{ 67,	"Ho",	0xFF000000,		0xFF000000,		0xFF000000,		0xFF000000,		0xFF000000,			0xFF000000,			"Holmium",			"Stockholm, Sweden (with the Latin name Holmia)",																											0,			6,			164.93032,		8.795,		1734,			2993,			0.165,		1.23,					1.3			},
		{ 68,	"Er",	0xFF000000,		0xFF000000,		0xFF000000,		0xFF000000,		0xFF000000,			0xFF000000,			"Erbium",			"Ytterby, Sweden",																																			0,			6,			167.259,		9.066,		1802,			3141,			0.168,		1.24,					3.5			},
		{ 69,	"Tm",	0xFF000000,		0xFF000000,		0xFF000000,		0xFF000000,		0xFF000000,			0xFF000000,			"Thulium",			"Thule, the ancient name for Scandinavia",																													0,			6,			168.93421,		9.321,		1818,			2223,			0.16,		1.25,					0.52		},
		{ 70,	"Yb",	0xFF000000,		0xFF000000,		0xFF000000,		0xFF000000,		0xFF000000,			0xFF000000,			"Ytterbium",		"Ytterby, Sweden",																																			0,			6,			173.054,		6.965,		1097,			1469,			0.155,		1.1,					3.2			},
		{ 71,	"Lu",	0xFF000000,		0xFF000000,		0xFF000000,		0xFF000000,		0xFF000000,			0xFF000000,			"Lutetium",			"Paris, France (with the Roman name Lutetia)",																												3,			6,			174.9668,		9.84,		1925,			3675,			0.154,		1.27,					0.8			},
		{ 72,	"Hf",	0xFF000000,		0xFF000000,		0xFF000000,		0xFF000000,		0xFF000000,			0xFF000000,			"Hafnium",			"Copenhagen, Denmark (with the Latin name Hafnia)",																											4,			6,			178.49,			13.31,		2506,			4876,			0.144,		1.3,					3			},
		{ 73,	"Ta",	0xFF000000,		0xFF000000,		0xFF000000,		0xFF000000,		0xFF000000,			0xFF000000,			"Tantalum",			"King Tantalus, father of Niobe from Greek mythology",																										5,			6,			180.94788,		16.654,		3290,			5731,			0.14,		1.5,					2			},
		{ 74,	"W",	0xFF000000,		0xFF000000,		0xFF000000,		0xFF000000,		0xFF000000,			0xFF000000,			"Tungsten",			"the Swedish 'tung sten' meaning heavy stone (W is wolfram, the old name of the tungsten mineral wolframite)[2]",											6,			6,			183.84,			19.25,		3695,			5828,			0.132,		2.36,					1.3			},
		{ 75,	"Re",	0xFF000000,		0xFF000000,		0xFF000000,		0xFF000000,		0xFF000000,			0xFF000000,			"Rhenium",			"Rhine, a river that flows from Grisons in the eastern Swiss Alps to the North Sea coast in the Netherlands (with the Latin name Rhenia)",					7,			6,			186.207,		21.02,		3459,			5869,			0.137,		1.9,					0.0001		},
		{ 76,	"Os",	0xFF000000,		0xFF000000,		0xFF000000,		0xFF000000,		0xFF000000,			0xFF000000,			"Osmium",			"the Greek 'osme', meaning smell",																															8,			6,			190.23,			22.61,		3306,			5285,			0.13,		2.2,					0.002		},
		{ 77,	"Ir",	0xFF000000,		0xFF000000,		0xFF000000,		0xFF000000,		0xFF000000,			0xFF000000,			"Iridium",			"Iris, the Greek goddess of the rainbow",																													9,			6,			192.217,		22.56,		2719,			4701,			0.131,		2.2,					0.001		},
		{ 78,	"Pt",	0xFF000000,		0xFF000000,		0xFF000000,		0xFF000000,		0xFF000000,			0xFF000000,			"Platinum",			"the Spanish 'platina', meaning little silver",																												10,			6,			195.084,		21.46,		2041.4,			4098,			0.133,		2.28,					0.005		},
		{ 79,	"Au",	0xFF000000,		0xFF000000,		0xFF000000,		0xFF000000,		0xFF000000,			0xFF000000,			"Gold",				"the Anglo-Saxon word gold (aurum in Latin, meaning glow of sunrise)[2]",																					11,			6,			196.966569,		19.282,		1337.33,		3129,			0.129,		2.54,					0.004		},
		{ 80,	"Hg",	0xFF000000,		0xFF000000,		0xFF000000,		0xFF000000,		0xFF000000,			0xFF000000,			"Mercury",			"Mercury, the first planet in the Solar System (Hg from former name hydrargyrum, from Greek hydr- water and argyros silver)",								12,			6,			200.592,		13.5336,	234.43,			629.88,			0.14,		2,						0.085		},
		{ 81,	"Tl",	0xFF000000,		0xFF000000,		0xFF000000,		0xFF000000,		0xFF000000,			0xFF000000,			"Thallium",			"the Greek 'thallos', meaning a green twig",																												13,			6,			204.389,		11.85,		577,			1746,			0.129,		1.62,					0.85		},
		{ 82,	"Pb",	0xFF000000,		0xFF000000,		0xFF000000,		0xFF000000,		0xFF000000,			0xFF000000,			"Lead",				"the Anglo-Saxon lead (plumbum in Latin)[2]",																												14,			6,			207.2,			11.342,		600.61,			2022,			0.129,		1.87,					14			},
		{ 83,	"Bi",	0xFF000000,		0xFF000000,		0xFF000000,		0xFF000000,		0xFF000000,			0xFF000000,			"Bismuth",			"the German 'Bisemutum' a corruption of 'Weisse Masse' meaning white mass",																					15,			6,			208.98040,		9.807,		544.7,			1837,			0.122,		2.02,					0.009		},
		{ 84,	"Po",	0xFF000000,		0xFF000000,		0xFF000000,		0xFF000000,		0xFF000000,			0xFF000000,			"Polonium",			"Poland, the native country of Marie Curie, who first isolated the element",																				16,			6,			209,			9.32,		527,			1235,			-1,			2.0,					0.0001		},
		{ 85,	"At",	0xFF000000,		0xFF000000,		0xFF000000,		0xFF000000,		0xFF000000,			0xFF000000,			"Astatine",			"the Greek 'astatos', meaning unstable",																													17,			6,			210,			7,			575,			610,			-1,			2.2,					0.0001		},
		{ 86,	"Rn",	0xFF000000,		0xFF000000,		0xFF000000,		0xFF000000,		0xFF000000,			0xFF000000,			"Radon",			"From radium, as it was first detected as an emission from radium during radioactive decay",																18,			6,			222,			0.00973,	202,			211.3,			0.094,		2.2,					0.0001		},
		{ 87,	"Fr",	0xFF000000,		0xFF000000,		0xFF000000,		0xFF000000,		0xFF000000,			0xFF000000,			"Francium",			"France, where it was first discovered",																													1,			7,			223,			1.87,		300,			950,			-1,			0.7,					0.0001		},
		{ 88,	"Ra",	0xFF000000,		0xFF000000,		0xFF000000,		0xFF000000,		0xFF000000,			0xFF000000,			"Radium",			"the Latin 'radius', meaning ray",																															2,			7,			226,			5.5,		973,			2010,			0.094,		0.9,					0.0001		},
		//-------------------------------------------------------------------------
		// N	// Sym	//ColorSolid	// ColorLiquid	// ColorGas		// ColorCrystal	// ColorElectricMax	// ColorElectricMin	// Name				// NameOrigin																																				// Group	// Period	// AtomicWeight	// Density	// MeltPoint	// BoilPoint	// Heat		// Electronegativity	// Abundance
		{ 89,	"Ac",	0xFF000000,		0xFF000000,		0xFF000000,		0xFF000000,		0xFF000000,			0xFF000000,			"Actinium",			"the Greek 'actinos', meaning a ray",																														0,			7,			227,			10.07,		1323,			3471,			0.12,		1.1,					0.0001		},
		{ 90,	"Th",	0xFF000000,		0xFF000000,		0xFF000000,		0xFF000000,		0xFF000000,			0xFF000000,			"Thorium",			"Thor, the Scandinavian god of thunder",																													0,			7,			232.03806,		11.72,		2115,			5061,			0.113,		1.3,					9.6			},
		{ 91,	"Pa",	0xFF000000,		0xFF000000,		0xFF000000,		0xFF000000,		0xFF000000,			0xFF000000,			"Protactinium",		"the Greek 'protos', meaning first, as a prefix to the element actinium, which is produced through the radioactive decay of protactinium",					0,			7,			231.03588,		15.37,		1841,			4300,			-1,			1.5,					0.0001		},
		{ 92,	"U",	0xFF000000,		0xFF000000,		0xFF000000,		0xFF000000,		0xFF000000,			0xFF000000,			"Uranium",			"Uranus, the seventh planet in the Solar System",																											0,			7,			238.02891,		18.95,		1405.3,			4404,			0.116,		1.38,					2.7			},
		{ 93,	"Np",	0xFF000000,		0xFF000000,		0xFF000000,		0xFF000000,		0xFF000000,			0xFF000000,			"Neptunium",		"Neptune, the eighth planet in the Solar System",																											0,			7,			237,			20.45,		917,			4273,			-1,			1.36,					0.0001		},
		{ 94,	"Pu",	0xFF000000,		0xFF000000,		0xFF000000,		0xFF000000,		0xFF000000,			0xFF000000,			"Plutonium",		"Pluto, a dwarf planet in the Solar System",																												0,			7,			244,			19.84,		912.5,			3501,			-1,			1.28,					0.0001		},
		{ 95,	"Am",	0xFF000000,		0xFF000000,		0xFF000000,		0xFF000000,		0xFF000000,			0xFF000000,			"Americium",		"Americas, the continent where the element was first synthesized",																							0,			7,			243,			13.69,		1449,			2880,			-1,			1.13,					0.0001		},
		{ 96,	"Cm",	0xFF000000,		0xFF000000,		0xFF000000,		0xFF000000,		0xFF000000,			0xFF000000,			"Curium",			"Pierre Curie, a physicist, and Marie Curie, a physicist and chemist",																						0,			7,			247,			13.51,		1613,			3383,			-1,			1.28,					0.0001		},
		{ 97,	"Bk",	0xFF000000,		0xFF000000,		0xFF000000,		0xFF000000,		0xFF000000,			0xFF000000,			"Berkelium",		"Berkeley, California, USA, where the element was first synthesized",																						0,			7,			247,			14.79,		1259,			2900,			-1,			1.3,					0.0001		},
		{ 98,	"Cf",	0xFF000000,		0xFF000000,		0xFF000000,		0xFF000000,		0xFF000000,			0xFF000000,			"Californium",		"State of California, USA, where the element was first synthesized",																						0,			7,			251,			15.1,		1173,			1743,			-1,			1.3,					0.0001		},
		{ 99,	"Es",	0xFF000000,		0xFF000000,		0xFF000000,		0xFF000000,		0xFF000000,			0xFF000000,			"Einsteinium",		"Albert Einstein, physicist",																																0,			7,			252,			8.84,		1133,			1269,			-1,			1.3,					0			},
		{ 100,	"Fm",	0xFF000000,		0xFF000000,		0xFF000000,		0xFF000000,		0xFF000000,			0xFF000000,			"Fermium",			"Enrico Fermi, physicist",																																	0,			7,			257,			-1,			1125,			-1,				-1,			1.3,					0			},
		{ 101,	"Md",	0xFF000000,		0xFF000000,		0xFF000000,		0xFF000000,		0xFF000000,			0xFF000000,			"Mendelevium",		"Dmitri Mendeleyev, chemist and inventor",																													0,			7,			258,			-1,			1100,			-1,				-1,			1.3,					0			},
		{ 102,	"No",	0xFF000000,		0xFF000000,		0xFF000000,		0xFF000000,		0xFF000000,			0xFF000000,			"Nobelium",			"Alfred Nobel, chemist, engineer, innovator, and armaments manufacturer",																					0,			7,			259,			-1,			1100,			-1,				-1,			1.3,					0			},
		{ 103,	"Lr",	0xFF000000,		0xFF000000,		0xFF000000,		0xFF000000,		0xFF000000,			0xFF000000,			"Lawrencium",		"Ernest O. Lawrence, physicist",																															3,			7,			262,			-1,			1900,			-1,				-1,			1.3,					0			},
		{ 104,	"Rf",	0xFF000000,		0xFF000000,		0xFF000000,		0xFF000000,		0xFF000000,			0xFF000000,			"Rutherfordium",	"Ernest Rutherford, chemist and physicist",																													4,			7,			267,			23.2,		2400,			5800,			-1,			-1,						0			},
		{ 105,	"Db",	0xFF000000,		0xFF000000,		0xFF000000,		0xFF000000,		0xFF000000,			0xFF000000,			"Dubnium",			"Dubna, Russia",																																			5,			7,			268,			29.3,		-1,				-1,				-1,			-1,						0			},
		{ 106,	"Sg",	0xFF000000,		0xFF000000,		0xFF000000,		0xFF000000,		0xFF000000,			0xFF000000,			"Seaborgium",		"Glenn T. Seaborg, scientist",																																6,			7,			269,			35.0,		-1,				-1,				-1,			-1,						0			},
		{ 107,	"Bh",	0xFF000000,		0xFF000000,		0xFF000000,		0xFF000000,		0xFF000000,			0xFF000000,			"Bohrium",			"Niels Bohr, physicist",																																	7,			7,			270,			37.1,		-1,				-1,				-1,			-1,						0			},
		{ 108,	"Hs",	0xFF000000,		0xFF000000,		0xFF000000,		0xFF000000,		0xFF000000,			0xFF000000,			"Hassium",			"Hesse, Germany, where the element was first synthesized",																									8,			7,			269,			40.7,		-1,				-1,				-1,			-1,						0			},
		{ 109,	"Mt",	0xFF000000,		0xFF000000,		0xFF000000,		0xFF000000,		0xFF000000,			0xFF000000,			"Meitnerium", 		"Lise Meitner, physicist",																																	9,			7,			278,			37.4,		-1,				-1,				-1,			-1,						0			},
		{ 110,	"Ds",	0xFF000000,		0xFF000000,		0xFF000000,		0xFF000000,		0xFF000000,			0xFF000000,			"Darmstadtium",		"Darmstadt, Germany, where the element was first synthesized",																								10,			7,			281,			34.8,		-1,				-1,				-1,			-1,						0			},
		{ 111,	"Rg",	0xFF000000,		0xFF000000,		0xFF000000,		0xFF000000,		0xFF000000,			0xFF000000,			"Roentgenium",		"Wilhelm Conrad Röntgen, physicist",																														11,			7,			281,			28.7,		-1,				-1,				-1,			-1,						0			},
		{ 112,	"Cn",	0xFF000000,		0xFF000000,		0xFF000000,		0xFF000000,		0xFF000000,			0xFF000000,			"Copernicium",		"Nicolaus Copernicus, astronomer",																															12,			7,			285,			23.7,		-1,				357,			-1,			-1,						0			},
		{ 113,	"Uut",	0xFF000000,		0xFF000000,		0xFF000000,		0xFF000000,		0xFF000000,			0xFF000000,			"Ununtrium",		"IUPAC systematic element name",																															13,			7,			286,			16,			700,			1400,			-1,			-1,						0			},
		{ 114,	"Fl",	0xFF000000,		0xFF000000,		0xFF000000,		0xFF000000,		0xFF000000,			0xFF000000,			"Flerovium",		"Georgy Flyorov, physicist",																																14,			7,			289,			14,			340,			420,			-1,			-1,						0			},
		{ 115,	"Uup",	0xFF000000,		0xFF000000,		0xFF000000,		0xFF000000,		0xFF000000,			0xFF000000,			"Ununpentium",		"IUPAC systematic element name",																															15,			7,			288,			13.5,		700,			1400,			-1,			-1,						0			},
		{ 116,	"Lv",	0xFF000000,		0xFF000000,		0xFF000000,		0xFF000000,		0xFF000000,			0xFF000000,			"Livermorium",		"Lawrence Livermore National Laboratory (in Livermore, California) which collaborated with JINR on its synthesis",											16,			7,			293,			12.9,		708.5,			1085,			-1,			-1,						0			},
		{ 117,	"Uus",	0xFF000000,		0xFF000000,		0xFF000000,		0xFF000000,		0xFF000000,			0xFF000000,			"Ununseptium",		"IUPAC systematic element name",																															17,			7,			294,			7.2,		673,			823,			-1,			-1,						0			},
		{ 118,	"Uuo",	0xFF000000,		0xFF000000,		0xFF000000,		0xFF000000,		0xFF000000,			0xFF000000,			"Ununoctium",		"IUPAC systematic element name",																															18,			7,			294,			5.0,		258,			263,			-1,			-1,						0			}
	}; // g_PeriodicTable

	typedef uint8_t GAMINOACID_TYPE;
	static const GAMINOACID_TYPE GAMINOACIDTYPE_UNKNOWN			= 0;
	// ------------------------ Small
	static const GAMINOACID_TYPE GAMINOACIDTYPE_GLYCINE			= 1;
	static const GAMINOACID_TYPE GAMINOACIDTYPE_ALANINE			= 2;
	// ------------------------ Nucleophilic
	static const GAMINOACID_TYPE GAMINOACIDTYPE_SERINE			= 3;
	static const GAMINOACID_TYPE GAMINOACIDTYPE_THREONINE		= 4;
	static const GAMINOACID_TYPE GAMINOACIDTYPE_CYSTEINE		= 5;
	// ------------------------ Hydrophobic
	static const GAMINOACID_TYPE GAMINOACIDTYPE_VALINE			= 6;
	static const GAMINOACID_TYPE GAMINOACIDTYPE_LEUCINE			= 7;
	static const GAMINOACID_TYPE GAMINOACIDTYPE_ISOLEUCINE		= 8;
	static const GAMINOACID_TYPE GAMINOACIDTYPE_METHIONINE		= 9;
	static const GAMINOACID_TYPE GAMINOACIDTYPE_PROLINE			= 10;
	// ------------------------ Aromatic
	static const GAMINOACID_TYPE GAMINOACIDTYPE_PHENYLALANINE	= 11;
	static const GAMINOACID_TYPE GAMINOACIDTYPE_TYROSINE		= 12;
	static const GAMINOACID_TYPE GAMINOACIDTYPE_TRYPTOPHAN		= 13;
	// ------------------------ Acidic
	static const GAMINOACID_TYPE GAMINOACIDTYPE_ASPARTICACID	= 14;
	static const GAMINOACID_TYPE GAMINOACIDTYPE_GLUTAMICACID	= 15;
	// ------------------------ Amide
	static const GAMINOACID_TYPE GAMINOACIDTYPE_ASPARAGINE		= 16;
	static const GAMINOACID_TYPE GAMINOACIDTYPE_GLUTAMINE		= 17;
	// ------------------------ Basic
	static const GAMINOACID_TYPE GAMINOACIDTYPE_HISTIDINE		= 18;
	static const GAMINOACID_TYPE GAMINOACIDTYPE_LYSINE			= 19;
	static const GAMINOACID_TYPE GAMINOACIDTYPE_ARGININE		= 20;
	// ------------------------ 
	static const GAMINOACID_TYPE GAMINOACIDTYPE_SELENOCYSTEINE	= 21;
	static const GAMINOACID_TYPE GAMINOACIDTYPE_ORNITHINE		= 22;

	GDECLARE_CORE_STRUCT( AMINOACID, Aminoacid, 
		GAMINOACID_TYPE	Type;
		char			Letter;
		char			AtomCount;
		atom_t			AtomsAtomicNumber[32];
		char			AtomsSymbol[32];
		char			AtomsPacked[30];
		char			InternalName[64];
		char			CommonName[64];
	);
	static void onCreateAminoacidDefault(ggns_Aminoacid* stuff)
	{
		memset((*stuff).get_data_address(), 0, (*stuff).get_data_size());
	};


#define INVALID_AMINOACID { 0,								'?',		0,				{0}														,	""								,	""				,	"UNKNOWN"			,	"Unknown"			}

	static const SAminoacid KnownAminoacids[32] = {
		// Type								// Letter	// AtomCount	// AtomsAtomicNumber								
		INVALID_AMINOACID,
		{ GAMINOACIDTYPE_GLYCINE,			'G',		10,				{ 6,6,1,1,1,1,1,7,8,8									},	"CCHHHHHNOO"					,	"C2H5NO2"		,	"GLYCINE"			,	"Glycine"			},
		{ GAMINOACIDTYPE_ALANINE,			'A',		13,				{ 6,6,6,1,1,1,1,1,1,1,7,8,8								},	"CCCHHHHHHHNOO"					,	"C3H7NO2"		,	"ALANINE"			,	"Alanine"			},
		{ GAMINOACIDTYPE_SERINE,			'S',		14,				{ 6,6,6,1,1,1,1,1,1,1,7,8,8,8							},	"CCCHHHHHHHNOOO"				,	"C3H7NO3"		,	"SERINE"			,	"Serine"			},
		{ GAMINOACIDTYPE_THREONINE,			'T',		17,				{ 6,6,6,6,1,1,1,1,1,1,1,1,1,7,8,8,8						},	"CCCCHHHHHHHHHNOOO"				,	"C4H9NO3"		,	"THREONINE"			,	"Threonine"			},
		{ GAMINOACIDTYPE_CYSTEINE,			'C',		14,				{ 6,6,6,1,1,1,1,1,1,1,7,8,8,16							},	"CCCHHHHHHHNOOS"				,	"C3H7NO2S"		,	"CYSTEINE"			,	"Cysteine"			},
		{ GAMINOACIDTYPE_VALINE,			'V',		19,				{ 6,6,6,6,6,1,1,1,1,1,1,1,1,1,1,1,7,8,8					},	"CCCCCHHHHHHHHHHHNOO"			,	"C5H11NO2"		,	"VALINE"			,	"Valine"			},
		{ GAMINOACIDTYPE_LEUCINE,			'L',		22,				{ 6,6,6,6,6,6,1,1,1,1,1,1,1,1,1,1,1,1,1,7,8,8			},	"CCCCCCHHHHHHHHHHHHHNOO"		,	"C6H13NO2"		,	"LEUCINE"			,	"Leucine"			},
		{ GAMINOACIDTYPE_ISOLEUCINE,		'I',		22,				{ 6,6,6,6,6,6,1,1,1,1,1,1,1,1,1,1,1,1,1,7,8,8			},	"CCCCCCHHHHHHHHHHHHHNOO"		,	"C6H13NO2"		,	"ISOLEUCINE"		,	"Isoleucine"		},
		{ GAMINOACIDTYPE_METHIONINE,		'M',		20,				{ 6,6,6,6,6,1,1,1,1,1,1,1,1,1,1,1,7,8,8,16				},	"CCCCCHHHHHHHHHHHNOOS"			,	"C5H11NO2S"		,	"METHIONINE"		,	"Methionine"		},
		{ GAMINOACIDTYPE_PROLINE,			'P',		17,				{ 6,6,6,6,6,1,1,1,1,1,1,1,1,1,7,8,8						},	"CCCCCHHHHHHHHHNOO"				,	"C5H9NO2"		,	"PROLINE"			,	"Proline"			},
		{ GAMINOACIDTYPE_PHENYLALANINE,		'F',		23,				{ 6,6,6,6,6,6,6,6,6,1,1,1,1,1,1,1,1,1,1,1,7,8,8			},	"CCCCCCCCCHHHHHHHHHHHNOO"		,	"C9H11NO2"		,	"PHENYLALANINE"		,	"Phenylalanine"		},
		{ GAMINOACIDTYPE_TYROSINE,			'Y',		24,				{ 6,6,6,6,6,6,6,6,6,1,1,1,1,1,1,1,1,1,1,1,7,8,8,8		},	"CCCCCCCCCHHHHHHHHHHHNOOO"		,	"C9H11NO3"		,	"TYROSINE"			,	"Tyrosine"			},
		{ GAMINOACIDTYPE_TRYPTOPHAN,		'W',		27,				{ 6,6,6,6,6,6,6,6,6,6,6,1,1,1,1,1,1,1,1,1,1,1,1,7,7,8,8	},	"CCCCCCCCCCCHHHHHHHHHHHHNNOO"	,	"C11H12N2O2"	,	"TRYPTOPHAN"		,	"Tryptophan"		},
		{ GAMINOACIDTYPE_ASPARTICACID,		'D',		16,				{ 6,6,6,6,1,1,1,1,1,1,1,7,8,8,8,8						},	"CCCCHHHHHHHNOOOO"				,	"C4H7NO4"		,	"ASPARTICACID"		,	"Aspartic acid"		},
		{ GAMINOACIDTYPE_GLUTAMICACID,		'E',		19,				{ 6,6,6,6,6,1,1,1,1,1,1,1,1,1,7,8,8,8,8					},	"CCCCCHHHHHHHHHNOOOO"			,	"C5H9NO4"		,	"GLUTAMICACID"		,	"Glutamic acid"		},
		{ GAMINOACIDTYPE_ASPARAGINE,		'N',		17,				{ 6,6,6,6,1,1,1,1,1,1,1,1,7,7,8,8,8						},	"CCCCHHHHHHHHNNOOO"				,	"C4H8N2O3"		,	"ASPARAGINE"		,	"Asparagine"		},
		{ GAMINOACIDTYPE_GLUTAMINE,			'Q',		20,				{ 6,6,6,6,6,1,1,1,1,1,1,1,1,1,1,7,7,8,8,8				},	"CCCCCHHHHHHHHHHNNOOO"			,	"C5H10N2O3"		,	"GLUTAMINE"			,	"Glutamine"			},
		{ GAMINOACIDTYPE_HISTIDINE,			'H',		20,				{ 6,6,6,6,6,6,1,1,1,1,1,1,1,1,1,7,7,7,8,8				},	"CCCCCCHHHHHHHHHNNNOO"			,	"C6H9N3O2"		,	"HISTIDINE"			,	"Histidine"			},
		{ GAMINOACIDTYPE_LYSINE,			'K',		24,				{ 6,6,6,6,6,6,1,1,1,1,1,1,1,1,1,1,1,1,1,1,7,7,8,8		},	"CCCCCCHHHHHHHHHHHHHHNNOO"		,	"C6H14N2O2"		,	"LYSINE"			,	"Lysine"			},
		{ GAMINOACIDTYPE_ARGININE,			'R',		26,				{ 6,6,6,6,6,6,1,1,1,1,1,1,1,1,1,1,1,1,1,1,7,7,7,7,8,8	},	"CCCCCCHHHHHHHHHHHHHHNNNNOO"	,	"C6H14N4O2"		,	"ARGININE"			,	"Arginine"			},
		{ GAMINOACIDTYPE_SELENOCYSTEINE,	'U',		14,				{ 6,6,6,1,1,1,1,1,1,1,7,8,8,34							},	"CCCHHHHHHHNOOSe"				,	"C3H7NO2Se"		,	"SELENOCYSTEINE"	,	"Selenocysteine"	},
		//{ GAMINOACIDTYPE_ORNITHINE,		'O',		21,				{ 6,6,6,6,6,1,1,1,1,1,1,1,1,1,1,1,1,7,7,8,8				},	"CCCCCHHHHHHHHHHHHNNOO"			,	"C5H12N2O2"		,	"ORNITHINE"			,	"Ornithine"			},
		INVALID_AMINOACID
	};


	// ---------------------------- Intramolecular ("strong") ----------------------------------------
	// ----- Metallic bonds 	
    //		- Metal aromaticity
	// ----- Covalent bonds 	
	//	- By symmetry 	
	//		- Sigma (σ)
	//		- Pi (π)
	//		- Delta (δ)
	//		- Phi (φ)
	// 
	//	- By multiplicity 	
    //		- 1 (single)
    //		- 2 (double)
    //		- 3 (triple)
    //		- 4 (quadruple)
    //		- 5 (quintuple)
    //		- 6 (sextuple)
	//
	//	- Miscellaneous 	
	// 
    //		- Agostic bond
    //		- Bent bond
    //		- Dipolar bond
    //		- Pi backbond
	//
    //		- Hapticity
    //		- Conjugation
    //		- Hyperconjugation
    //		- Antibonding
	//
	// - Resonant bonds 	
	// 		- Electron-deficient
	// 			- 3c–2e
	// 			- 4c–2e
	//
    //		- Hypercoordination
	//			- 3c–4e
    //		- Aromaticity
	// -----------------------------------------------------
	// -----------------------------------------------------
	// ----- Ionic bonding 	
	// ---------------------------- Intermolecular ("weak") ----------------------------------------
	// 
	//	- van der Waals forces 	
	//		- London dispersion force
	//
	//	- Hydrogen bond 	
    //		- Low-barrier
    //		- Resonance-assisted
    //		- Symmetric
    //		- Dihydrogen bonds
	//
	// - Other noncovalent 	
	//		- Mechanical bond
    //		- Halogen bond
    //		- Aurophilicity
    //		- Intercalation
    //		- Stacking
    //		- Cation–pi bond
    //		- Anion–pi bond
    //		- Salt bridge
	typedef uint8_t GBOND_TYPE;
	static const GBOND_TYPE GBONDTYPE_NONE						= 0;
	
	static const GBOND_TYPE GBONDTYPE_INTRAMOLECULAR			= 0x00;
	static const GBOND_TYPE GBONDTYPE_INTERMOLECULAR			= 0x80;

	static const GBOND_TYPE GBONDTYPE_METALLIC					= 0x10;
	static const GBOND_TYPE GBONDTYPE_IONIC						= 0x20;
	static const GBOND_TYPE GBONDTYPE_COVALENT					= 0x40;

	static const GBOND_TYPE GBONDTYPE_VANDERWAALS				= 0x10;
	static const GBOND_TYPE GBONDTYPE_HYDROGEN					= 0x20;
	static const GBOND_TYPE GBONDTYPE_OTHER						= 0x40;

	// Metallic 
	// Ionic
	// Covalent
	GDECLARE_CORE_STRUCT( BOND, Bond, 
		GBOND_TYPE	Type;
		uint64_t	SectionA;
		uint64_t	IndexA;
		uint64_t	SectionB;
		uint64_t	IndexB;
	);

	static void onCreateBondDefault(ggns_Bond* stuff)
	{
		memset((*stuff).get_data_address(), 0, (*stuff).get_data_size());
	};
#pragma pack(pop)

} // namespace

#endif // _GATOM_CORE_H
