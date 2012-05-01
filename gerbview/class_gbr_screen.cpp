/**
 * @file classpcb.cpp
 * @brief Member functions of classes used in Pcbnew (see pcbstruct.h)
 *        except for tracks (see class_track.cpp).
 */

#include <fctsys.h>
#include <common.h>
#include <macros.h>
#include <class_gbr_screen.h>
#include <base_units.h>

#include <pcbnew.h>
#include <class_board_design_settings.h>
#include <layers_id_colors_and_visibility.h>

#include <pcbnew_id.h>


#define DMIL_GRID( x ) wxRealPoint( x * IU_PER_DECIMILS,\
                                    x * IU_PER_DECIMILS )
#define MM_GRID( x )   wxRealPoint( x * IU_PER_MM,\
                                    x * IU_PER_MM )


/**
    Default Pcbnew zoom values.
    Limited to 19 values to keep a decent size to menus.
    Roughly a 1.5 progression.
    The last 2 values are handy when somebody uses a library import of a module
    (or foreign data) which has a bad coordinate.
    Also useful in GerbView for this reason.
    Zoom 5 and 10 can create artefacts when drawing (integer overflow in low level graphic
    functions )
*/
static const double gbrZoomList[] =
{
    ZOOM_FACTOR( 0.5 ),
    ZOOM_FACTOR( 1.0 ),
    ZOOM_FACTOR( 1.5 ),
    ZOOM_FACTOR( 2.0 ),
    ZOOM_FACTOR( 3.0 ),
    ZOOM_FACTOR( 4.5 ),
    ZOOM_FACTOR( 7.0 ),
    ZOOM_FACTOR( 10.0 ),
    ZOOM_FACTOR( 15.0 ),
    ZOOM_FACTOR( 22.0 ),
    ZOOM_FACTOR( 35.0 ),
    ZOOM_FACTOR( 50.0 ),
    ZOOM_FACTOR( 80.0 ),
    ZOOM_FACTOR( 120.0 ),
    ZOOM_FACTOR( 200.0 ),
    ZOOM_FACTOR( 350.0 ),
    ZOOM_FACTOR( 500.0 ),
    ZOOM_FACTOR( 1000.0 ),
    ZOOM_FACTOR( 2000.0 )
};


// Default grid sizes for PCB editor screens.
static GRID_TYPE gbrGridList[] =
{
    // predefined grid list in 0.0001 inches
    { ID_POPUP_GRID_LEVEL_1000,     DMIL_GRID( 1000 )  },
    { ID_POPUP_GRID_LEVEL_500,      DMIL_GRID( 500 )   },
    { ID_POPUP_GRID_LEVEL_250,      DMIL_GRID( 250 )   },
    { ID_POPUP_GRID_LEVEL_200,      DMIL_GRID( 200 )   },
    { ID_POPUP_GRID_LEVEL_100,      DMIL_GRID( 100 )   },
    { ID_POPUP_GRID_LEVEL_50,       DMIL_GRID( 50 )    },
    { ID_POPUP_GRID_LEVEL_25,       DMIL_GRID( 25 )    },
    { ID_POPUP_GRID_LEVEL_20,       DMIL_GRID( 20 )    },
    { ID_POPUP_GRID_LEVEL_10,       DMIL_GRID( 10 )    },
    { ID_POPUP_GRID_LEVEL_5,        DMIL_GRID( 5 )     },
    { ID_POPUP_GRID_LEVEL_2,        DMIL_GRID( 2 )     },
    { ID_POPUP_GRID_LEVEL_1,        DMIL_GRID( 1 )     },

    // predefined grid list in mm
    { ID_POPUP_GRID_LEVEL_5MM,      MM_GRID( 5.0 )     },
    { ID_POPUP_GRID_LEVEL_2_5MM,    MM_GRID( 2.5 )     },
    { ID_POPUP_GRID_LEVEL_1MM,      MM_GRID( 1.0 )     },
    { ID_POPUP_GRID_LEVEL_0_5MM,    MM_GRID( 0.5 )     },
    { ID_POPUP_GRID_LEVEL_0_25MM,   MM_GRID( 0.25 )    },
    { ID_POPUP_GRID_LEVEL_0_2MM,    MM_GRID( 0.2 )     },
    { ID_POPUP_GRID_LEVEL_0_1MM,    MM_GRID( 0.1 )     },
    { ID_POPUP_GRID_LEVEL_0_0_5MM,  MM_GRID( 0.05 )    },
    { ID_POPUP_GRID_LEVEL_0_0_25MM, MM_GRID( 0.025 )   },
    { ID_POPUP_GRID_LEVEL_0_0_1MM,  MM_GRID( 0.01 )    }
};


GBR_SCREEN::GBR_SCREEN( const wxSize& aPageSizeIU ) :
    PCB_SCREEN( aPageSizeIU )
{
    // Replace zoom and grid lists already set by PCB_SCREEN ctor
    m_ZoomList.Clear();
    for( unsigned i = 0; i < DIM( gbrZoomList );  ++i )
        m_ZoomList.Add( gbrZoomList[i] );

    GRIDS gridlist;
    for( unsigned i = 0; i < DIM( gbrGridList );  ++i )
        gridlist.push_back( gbrGridList[i] );
    SetGridList( gridlist );

    // Set the working grid size to a reasonnable value (in 1/10000 inch)
    SetGrid( DMIL_GRID( 500 ) );

    m_Active_Layer       = LAYER_N_BACK;      // default active layer = bottom layer

    SetZoom( ZOOM_FACTOR( 350 ) );            // a default value for zoom
}


GBR_SCREEN::~GBR_SCREEN()
{
}


// virtual function
int GBR_SCREEN::MilsToIuScalar()
{
    return (int)IU_PER_MILS;
}