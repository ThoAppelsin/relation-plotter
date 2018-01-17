#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <math.h>

/*
defined operators:
+ addition
- subtraction
+ unary plus
- unary minus
() grouping
[] grouping
{} grouping
* multiplication
/ division
^ power
% modulus
*/
typedef enum OperatorTag
{
	Self,
	Add,
	Subtract,
	UnaryPlus,
	UnaryMinus,
	Multiply,
	Divide,
	Power,
	Modulo
} Operator;

typedef struct ExpressionTag
{
	Operator o;
	int value;
	struct ExpressionTag * operand1;
	struct ExpressionTag * operand2;
} Expression;

Expression * NewExpression( )
{
	Expression * result = malloc( sizeof * result );
	result->o = Self;
	result->value = 0;
	result->operand1 = NULL;
	result->operand2 = NULL;

	return result;
}

int IntegerCalculator( char * input )
{
	Expression * Topmost = NewExpression( );
	Expression * ParentE = Topmost;
	Expression * Current = NULL;

	while ( input[0] )
	{
		while ( isspace( input[0] ) )
			input++;

		if ( isdigit( input[0] ) )
		{
			Current = NewExpression( );

			while ( isdigit( input[0] ) )
			{
				Current->o = Self;
				Current->value = 10 * Current->value + input[0] - '0';
				input++;
			}

			if ( ParentE->operand1 == NULL )
			{
				ParentE->operand1 = Current;
				
			}
			else if ( ParentE->operand2 == NULL )
				ParentE->operand2 = Current;

			Current = NULL;
		}
		else switch ( input[0] )
		{
			case '+':
				if ( ParentE->operand1 == NULL )
				{
					ParentE->o = UnaryPlus;
					
				}
				else
				{
					ParentE->o = Add;
				}
				break;
			case '-':
				ParentE->o = (ParentE->operand1 == NULL) ? Subtract : UnaryMinus;
				break;
			case '*':
				ParentE->o = Multiply;
				break;
			case '/':
				ParentE->o = Divide;
				break;
			case '%':
				ParentE->o = Modulo;
				break;
			case '^':
				ParentE->o = Power;
				break;
			default:
				// invalid input right here
				;
		}
	}

	return 0;
}

/*
generic warning: "invalid"

filename:    something
x-min:       something
x-max:       something
min has to be strictly less than max, try again
...
y-min:       something
y-max:       something
min has to be strictly less than max, try again
...
thickness:   something
axes? (y/n): something

equations (type 'done' to indicate you are done):
- something
- something
- something
- done

begun processing your image...
your image has been processed in 123 milliseconds
*/

char filename[FILENAME_MAX];
int xmin;
int xmax;
int ymin;
int ymax;
int thickness;
int shallprintaxes;
char ** equations;

void ReceivePreferences( void )
{
	char * AlignedPrompts[ ] =
	{
		"filename",
		"x-min",
		"x-max",
		"y-min",
		"y-max",
		"thickness",
		"axes? (y/n)"
	};
	size_t AlignedPromptsIdx = 0;
	size_t LongestLength = 0;
	char Buffer[BUFSIZ];

	for ( size_t i = 0; i < sizeof AlignedPrompts / sizeof * AlignedPrompts; i++ )
	{
		size_t OneLength = strlen( AlignedPrompts[i] );
		if ( OneLength > LongestLength )
			LongestLength = OneLength;
	}

	//plus 2 for the ':' and ' '
	LongestLength += 2;

	
a:	printf( "%-*s", LongestLength, AlignedPrompts[AlignedPromptsIdx] );
	fflush( stdout );
	fgets( filename, sizeof filename / sizeof * filename, stdin );

	//validate filename
	for ( size_t i = 0; filename[i]; i++ )
	{
		if ( filename[i] == '\n' )
		{
			if ( i == 0 )
			{
				puts( "filename may not be empty" );
				goto a;
			}
			filename[i] = '\0';
			break;
		}
		
		if ( !(isalnum( filename[i] ) || filename[i] == '_') )
		{
			printf( "filename may only consist of alphanumeric characters or underscores, no (%c)s\n", filename[i] );
			goto a;
		}
	}
	AlignedPromptsIdx++;

	printf( "%-*s", LongestLength, AlignedPrompts[AlignedPromptsIdx] );
	fflush( stdout );
	fgets( Buffer, sizeof Buffer / sizeof * Buffer, stdin );
}

double forsomeparabola( double y, double x )
{
	return y - x * x;
}

double forsomeellipse( double y, double x )
{
	return x * x + y * y * 2 - 10;
}

double forrad3circle( double y, double x )
{
	return y * y + x * x - 9;
}

int burnrate( double signeddistance, double tolerance, int scalemax )
{
	double distance = fabs( signeddistance );
	return (distance >= tolerance) ? 0 : ((int) (scalemax * (1.0 - distance / tolerance)));
}

int brightness( double y, double x, double tolerance, int scalemax )
{
	int bness = scalemax;

	double( *conditions[ ] )(double, double) =
	{
		forrad3circle,
		forsomeellipse,
		forsomeparabola
	};

	for ( size_t i = 0; i < sizeof conditions / sizeof *conditions; i++ )
	{
		bness -= burnrate( conditions[i]( x, y ), tolerance, scalemax );
		if ( bness <= 0 )
			return 0;
	}

	return bness;
}

int main( void )
{
	int xmin = -5;
	int xmax = 5;
	int ymin = -5;
	int ymax = 5;

	int ppu = 90;
	int scalemax = 100;

	FILE * output = fopen( "output.pgm", "w" );
	
	fputs( "P2\n", output );
	fprintf( output, "%d %d\n", ppu * (xmax - xmin) + 1, ppu * (ymax - ymin) + 1 );
	fprintf( output, "%d\n", scalemax );

	for ( int py = ymax * ppu; py >= ymin * ppu; py-- )
	{
		for ( int px = xmin * ppu; px <= xmax * ppu; px++ )
		{
			fprintf( output, "%d\n", brightness( (double) py / ppu, (double) px / ppu, 5.0 / ppu, scalemax ) );
		}
	}

	return 0;
}