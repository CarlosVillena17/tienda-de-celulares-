#include<iostream>
#include<fstream>
#include<iomanip>
#include<string.h>
#include<string>
#include<stdlib.h>
#include<conio.h>
#include<Windows.h>
using namespace std;

const char PRODUCTOS[]="PRODUCTOS.dat";
const char CLIENTE[]="CLIENTE.txt";
const char USUARIO[]="Fisi";
const char CONTRASENIA[]="Base19";


struct direccion
{
    char departamento[25];
    char provincia[25];
    char distrito[25];
    char casa[25];
    char referencia[40];
};

struct Cliente
{
    char nombre[25];
    char apellido[25];
    string dni;
    struct direccion d;
};


struct Producto
{
    char Marca[25];
    char Nombre[25];
    char Codigo[25];
    float Precio;
    int stock;
};

void gotoxy(int x, int y)
{
 HANDLE hcon;
 hcon = GetStdHandle(STD_OUTPUT_HANDLE);
 COORD dwPos;
 dwPos.X = x;
 dwPos.Y = y;
 SetConsoleCursorPosition(hcon,dwPos);
}
//Validacion de Entrada de Datos
float ValidarDatosFlotantes();
int ValidarDatosEnteros();

/*FUNCIONES */

//funciones para los clientes
void VerCarritoPorCliente();
bool ExistenciaDeUnCliente(string dnicliente);
void AgregarCelularAlCarrito();
void GuardarCliente();
Cliente PedirDatosClientes();


//funciones para admin
void Eliminar(char codigo[]);
void ModificarUnCelular(char codigo[]);
bool ExistenciaDeUnCelular(char codigo[]);
void MostrarTodosLosCelulares();
void Mostrar1Celular(Producto p);
void CabezeraDeCelulares();
void GuardarCelular();
Producto PedirDatosCelular();

//Menu De Opciones

void cliente();
void MenuDeAdmin();
void LogueComoAdmin();
void MenuPrincipal();

//INTERFAZ
void logodeventa();
void logocelular();
void interfaz();
void cargando(int x, int y, string nom);

int main(){
    interfaz();
    system("COLOR F0");
    MenuPrincipal();
    return 0;
}


Producto PedirDatosCelular(){
    Producto p; //definimos un dato de tipo Producto
    fflush(stdin);
    cout<<"\t\t  *INGRESAR MARCA PRODUCTO: ";cin.getline(p.Marca,25,'\n');
    fflush(stdin);
    cout<<"\t\t  *INGRESAR NOMBRE PRODUCTO: ";cin.getline(p.Nombre,25,'\n');
    fflush(stdin);
    cout<<"\t\t  *INGRESAR CODIGO PRODUCTO: ";cin.getline(p.Codigo,25,'\n');
    fflush(stdin);

    do
    {
        cout<<"\t\t  *INGRESAR PRECIO PRODUCTO: ";
        p.Precio=ValidarDatosFlotantes();
        fflush(stdin);

        if (p.Precio<=0)
        {
            cout<<"\t\t\t (DEBE INTRODUCIR UN VALOR VALIDO PARA EL PRECIO)"<<endl;
        }

    } while (p.Precio<=0);

   do
   {
        cout<<"\t\t  *INGRESAR STOCK: ";
        p.stock=ValidarDatosEnteros()  ;
        fflush(stdin);
        if (p.stock<=0)
        {
            cout<<"\t\t\t  (DEBE INTRODUCIR UNA CANTIDAD VALIDA)"<<endl;
        }

   } while (p.stock<=0);

   return p;  //Retornamos el dato Tipo Producto
}


void GuardarCelular(){
    int op;

    do
    {
        op=0;
        cout<<endl;
        cout<<"\t\t\t\t\t\t       REGISTRO DE CELULAR"<<endl;
        cout<<endl;
        Producto p;
        p=PedirDatosCelular();

        if (ExistenciaDeUnCelular(p.Codigo)==false)
        {
            ofstream GuardarProducto;
            GuardarProducto.open(PRODUCTOS,ios::binary|ios::app);

            if (GuardarProducto.good())
            {
                GuardarProducto.write((char*)&p,sizeof(Producto));
                cout<<"\t\t\t  CELULAR REGISTRADO CORRECTAMENTE "<<endl;
            }

            else
            {
                cout<<"\t\t\t (ERROR EN EL ARCHIVO DE CELULARES REGISTRADOS)"<<endl;
            }

        GuardarProducto.close();

        }

        else
        {
            cout<<"\t\t\t  ESTE CELULAR YA SE ENCUENTRA REGISTRADO , EVITE DUPLICARLO"<<endl;
            cout<<"\t\t\t  DESEA VOLVER A INTENTARLO: [SI]=1 , NO cualquier Tecla";
            op=ValidarDatosEnteros();
        }

    } while (op==1);

}


void CabezeraDeCelulares(){
    cout<<endl;
    cout<<setw(45)<<"MARCA"<<setw(25)<<"NOMBRE"<<setw(25)<<"CODIGO"<<setw(10)<<"PRECIO"<<setw(10)<<"STOCK"<<endl;
    cout<<setw(120)<<"--------------------------------------------------------------------------------------"<<endl;
}


void Mostrar1Celular(Producto p){
    cout<<setw(45)<<p.Marca<<setw(25)<<p.Nombre<<setw(25)<<p.Codigo<<setw(10)<<p.Precio<<setw(10)<<p.stock<<endl;
    cout<<setw(120)<<"_____________________________________________________________________________________"<<endl;
}


void MostrarTodosLosCelulares(){
    ifstream MostrarProducto;
    MostrarProducto.open(PRODUCTOS,ios::binary);

    if (MostrarProducto.good())
    {
        Producto p;
        CabezeraDeCelulares();

        while (MostrarProducto.read((char*)&p,sizeof(Producto)))
        {
            Mostrar1Celular(p);
        }

    }

    else
    {
        cout<<"(ERROR EN EL ARCHIVO DE CELULARES REGISTRADOS)"<<endl;
    }

    MostrarProducto.close();
}


bool ExistenciaDeUnCelular(char codigo[]){
    bool existe=false;
    ifstream MostrarProducto;
    MostrarProducto.open(PRODUCTOS,ios::binary);

    if (MostrarProducto.good())
    {
        Producto p;
        while (MostrarProducto.read((char*)&p,sizeof(Producto)) && existe==false)
        {
            if (strcmp(p.Codigo,codigo)==0)
            {
                existe=true;
            }
        }
    }

    else
    {
        cout<<"(ERROR EN EL ARCHIVO DE CELULARES REGISTRADOS)"<<endl;
    }

    MostrarProducto.close();
    return existe;
}


void ModificarUnCelular(char codigo[]){
    ifstream LeerProductos;
    ofstream Temporal;
    LeerProductos.open(PRODUCTOS,ios::binary);
    Temporal.open("temporal.txt",ios::binary|ios::app);

    if (LeerProductos.good()&& Temporal.good())
    {
        Producto p;

        while (LeerProductos.read((char*)&p,sizeof(Producto)))
        {
            if (strcmp(codigo,p.Codigo)==0)
            {
                cout<<endl;
                cout<<"\t\t\t\t\t\t\t   CELULAR A MODIFICAR"<<endl;
                cout<<"\t\t\t\t\t\t\t   -------------------"<<endl;
                cout<<"\t\t\t\t\t\t   NOMBRE DEL PRODUCTO: "<<p.Nombre<<endl;
                cout<<"\t\t\t\t\t\t     ---->PRECIO ACTUAL: "<<p.Precio<<endl;
                cout<<"\t\t\t\t\t\t     ---->STOCK ACTUAL:"<<p.stock<<endl;
                cout<<endl;
                                do
                                {
                                    cout<<"\t\t\t\t\t\t      INTRODUZCA NUEVO PRECIO: ";
                                    p.Precio=ValidarDatosFlotantes();
                                    fflush(stdin);

                                        if (p.Precio<=0)
                                        {
                                            cout<<"\t\t\t\t\t (DEBE INTRODUCIR UN VALOR VALIDO PARA EL NUEVO PRECIO)"<<endl;
                                        }

                                } while (p.Precio<=0);

                                cout<<endl;

                                do
                                {
                                    cout<<"\t\t\t\t\t\t      INTRODUZA NUEVO STOCK: ";
                                    p.stock=ValidarDatosEnteros()  ;
                                    fflush(stdin);

                                        if (p.stock<=0)
                                        {
                                        cout<<"\t\t\t\t\t\t  (DEBE INTRODUCIR UNA CANTIDAD VALIDA)"<<endl;
                                        }

                                } while (p.stock<=0);

                cout<<endl;
                cout<<endl;
                system("cls");
                cargando(45,5, "MODIFICANDO");
                cout<<"\t\t\t\t\t  ___________________________________"<<endl;
                cout<<"\t\t\t\t\t  ----CAMBIOS GUARDADOS CON EXITO----"<<endl;
                cout<<"\t\t\t\t\t  ___________________________________"<<endl;
            }

            Temporal.write((char*)&p,sizeof(Producto));

        }
    }

    else
    {
        cout<<"ERROR AL EN ARCHIVOS DE CELULARES Y EN EL TEMPORAL"<<endl;
        exit(1);
    }

    LeerProductos.close();
    Temporal.close();
    remove(PRODUCTOS);
    rename("temporal.txt",PRODUCTOS);
}


void Eliminar(char codigo[]){
    ifstream LeerProductos;
    ofstream Temporal;
    LeerProductos.open(PRODUCTOS,ios::binary);
    Temporal.open("temporal.txt",ios::binary|ios::app);

    if (LeerProductos.good()&&Temporal.good())
    {

        Producto p;
        while (LeerProductos.read((char*)&p,sizeof(Producto)))
        {

            if (strcmp(codigo,p.Codigo)==0)
            {    system("cls");
                cargando(45,5, "ELIMINANDO");
                cout<<"\n\t\t\t\t\t\tNOMBRE DEL PRODUCTO: "<<p.Nombre<<endl;
                cout<<"\n\t\t\t\t\t\tPRECIO ACTUAL: "<<p.Precio<<endl;
                cout<<"\n\t\t\t\t\t\tSTOCK ACTUAL:"<<p.stock<<endl;
                cout<<"\n\t\t\t\t\t\tPRODUCTO ELIMINADO CON EXITO"<<endl;
            }

            else
            {
                Temporal.write((char*)&p,sizeof(Producto));
            }
        }
    }

    else
    {
        cout<<"ERROR AL EN ARCHIVOS DE CELULARES Y EN EL TEMPORAL"<<endl;
        exit(1);
    }

    LeerProductos.close();
    Temporal.close();
    remove(PRODUCTOS);
    rename("temporal.txt",PRODUCTOS);
}



Cliente PedirDatosClientes(){
    Cliente c;
    fflush(stdin);
    cout<<endl;
    cout<<"\t\t\t\t\t\t     REGISTRO DE CLIENTE EN TIENDA CJURO-GONCA"<<endl;
    cout<<"\t\t\t\t\t\t     -----------------------------------------"<<endl;
    cout<<endl;
    cout<<"\t\t\t\t\t\t                    DATOS GENERALES"<<endl;
    cout<<"\t\t\t\t\t\t                    ---------------"<<endl;
    cout<<"\t\t\t\t      *NOMBRE: ";cin.getline(c.nombre,25,'\n');
    fflush(stdin);
    cout<<"\t\t\t\t    *APELLIDO: ";cin.getline(c.apellido,25,'\n');
    fflush(stdin);
    cout<<"\t\t\t\t         *DNI: ";getline(cin,c.dni);
    fflush(stdin);
    cout<<"\t\t\t\t\t\t                       DIRECCION"<<endl;
    cout<<"\t\t\t\t\t\t                       ---------"<<endl;
    cout<<"\t\t\t\t*DEPARTAMENTO: ";cin.getline(c.d.departamento,25,'\n');
    fflush(stdin);
    cout<<"\t\t\t\t   *PROVINCIA: ";cin.getline(c.d.distrito,25,'\n');
    fflush(stdin);
    cout<<"\t\t\t\t    *DISTRITO: ";cin.getline(c.d.distrito,25,'\n');
    fflush(stdin);
    cout<<"\t\t\t\t   *DIRECCION: ";cin.getline(c.d.casa,25,'\n');
    fflush(stdin);
    cout<<"\t\t\t\t  *REFERENCIA: ";cin.getline(c.d.referencia,50,'\n');
    fflush(stdin);
    cout<<endl;

    return c;
}

void GuardarCliente(){
    int op;
    do
    {
        system("cls");
        op=0;
        Cliente c;
        c=PedirDatosClientes();
        if (ExistenciaDeUnCliente(c.dni)==false)
        {
            ofstream GuardarCliente;
            GuardarCliente.open(CLIENTE);

            if (GuardarCliente.good())
            {
                fflush(stdin);
                GuardarCliente<<" DATOS DEL CLIENTE"<<endl;
                fflush(stdin);
                GuardarCliente<<"NOMBRE: "<<c.nombre<<endl;
                fflush(stdin);
                GuardarCliente<<"APELLIDO: "<<c.apellido<<endl;
                fflush(stdin);
                GuardarCliente<<"DNI: "<<c.dni<<endl;
                fflush(stdin);
                GuardarCliente<<"DATOS DE DIRECCION"<<endl;
                fflush(stdin);
                GuardarCliente<<c.d.departamento<<" "<<c.d.provincia<<" "<<c.d.distrito<<" "<<c.d.casa<<endl;
                GuardarCliente<<"REFERENCIA: "<<c.d.referencia<<endl;
                fflush(stdin);
                GuardarCliente<<"----------------------------"<<endl;
                GuardarCliente<<endl;
                fflush(stdin);
                GuardarCliente.close();
                fflush(stdin);
                c.dni=c.dni+".txt";
                rename(CLIENTE,c.dni.c_str());
            }

            else
            {
                cout<<"ERROR AL MOMENTO DE REGISTRAR"<<endl;
                GuardarCliente.close();
                remove(CLIENTE);
            }
        system("cls");
        cargando(45,5, "REGISTRANDO DATOS");
        cout<<"\n\t\t\t\t\t\tREGISTRO CORRECTO"<<endl;

        }

        else
        {
            cout<<"EL DNI "<<c.dni<<" YA SE ENCUENTRA REGISTRADO"<<endl;
            cout<<"INTENTAR OTRA VEZ SI[1] / NO [CUALQUIER TECLA]";
            op=ValidarDatosEnteros();
        }

    } while (op==1);

}


void AgregarCelularAlCarrito(){
    string dnicliente;
    fflush(stdin);
    cout<<endl;
    cout<<"\t\t\t\t\t\t                    AGREGAR CELULAR AL CARRITO"<<endl;
    cout<<"\t\t\t\t\t\t                    --------------------------"<<endl;
    cout<<endl;
    cout<<"\t\t\t\t*INGRESE SU DNI: ";getline(cin,dnicliente);

    if (ExistenciaDeUnCliente(dnicliente)==true)
    {
        cout<<endl;
        cout<<"\t\t\t\t\t\t\tLISTA DE CELULARES DISPONIBLES"<<endl;
        MostrarTodosLosCelulares();
        char codigo[25];
        int seguir=0;
        do
        {
            system("cls");
            fflush(stdin);
            cout<<"\t\t\t\t\t\t                    AGREGAR CELULAR AL CARRITO"<<endl;
            cout<<"\t\t\t\t\t\t                    --------------------------"<<endl;
            cout<<"\t\t\t\t\t\t\tLISTA DE CELULARES DISPONIBLES"<<endl;
            MostrarTodosLosCelulares();

            cout<<endl;
            cout<<"\t\t\t\t\t\t      ________________________________________"<<endl;
            cout<<"\t\t\t\t\t\t        BIENVENIDO USUARIO CON DNI "<<dnicliente<<endl;
            cout<<endl;
            cout<<"\t\t\t\t\tINGRESE EL CODIGO DEL CELULAR A ADQUIRIR: ";cin.getline(codigo,25,'\n');

            if (ExistenciaDeUnCelular(codigo)==true)
            {
            ifstream LeerProductos;
            ofstream Temporal;
            ofstream BoletaGuardar;
            LeerProductos.open(PRODUCTOS,ios::binary);
            Temporal.open("temporal.txt",ios::binary|ios::app);
            BoletaGuardar.open(dnicliente+".txt",ios::app);
                if (LeerProductos.good()&&Temporal.good()&&BoletaGuardar.good())
                {

                    Producto p;
                    while (LeerProductos.read((char*)&p,sizeof(Producto)))
                    {
                        if (strcmp(codigo,p.Codigo)==0)
                        {
                            int unidades;
                            int aux;

                            do
                            {
                            fflush(stdin);
                            unidades=0;
                            fflush(stdin);
                            cout<<"\t\t\t\t\tUNIDADES A AGREGAR AL CARRITO: ";cin>>unidades;
                            aux=p.stock-unidades;
                                if (aux<0)
                                {
                                    cout<<"\t\t\t\t\tNO SE DISPONE DE ESE STOCK "<<endl;
                                }

                            } while (aux<0);

                        p.stock=aux;
                        BoletaGuardar<<"BOLETA"<<endl;
                        BoletaGuardar<<"MODELO: "<<p.Nombre<<endl;
                        BoletaGuardar<<"CODIGO: "<<p.Codigo<<endl;
                        BoletaGuardar<<"PRECIO UNITARIO: "<<p.Precio<<endl;
                        BoletaGuardar<<"UNIDADES A LLEVAR: "<<unidades<<endl;
                        BoletaGuardar<<endl;
                        fflush(stdin);
                    }

                    Temporal.write((char*)&p,sizeof(Producto));
                }
                system("cls");
                cargando(40,5,"AGREGANDO" );
                cout<<endl;
                cout<<"\t\t\t\t\t  AGREGADO CON EXITO"<<endl;
            }

                else
                {
                cout<<"ERROR EN LOS ARCHIVOS"<<endl;
                remove("temporal.txt");
                exit(1);
                }

                LeerProductos.close();
                Temporal.close();
                BoletaGuardar.close();
                remove(PRODUCTOS);
                rename("temporal.txt",PRODUCTOS);
                cout<<"\t\t\t\t\tDESEA AGREGAR OTRO CELULAR Si[1] / NO [0]: ";

                cin>>seguir;

            }


            else
            {
            cout<<endl;
            cout<<"\t\t\t\t\t\t\t\t\tCODIGO NO EXISTE"<<endl;
            cout<<"\t\t\t\t\tDESEA VOLVER A INTRODUCIR OTRO CODIGO SI [1] / NO [0]" ;
            cin>>seguir;
            }

            if(seguir!=1){
                cout<<"\t\t\t\t\tGRACIAS POR LA COMPRA"<<endl;
            }

        } while (seguir==1);
    }


    else
    {
        cout<<endl;
        cout<<"\t\t\t\t\t\tUSTED NO ESTA REGISTRADO , REGISTRETESE"<<endl;
    }

}

bool ExistenciaDeUnCliente(string dnicliente){
    dnicliente=dnicliente+".txt";
    ifstream BuscandoSiExistrArchivo;
    BuscandoSiExistrArchivo.open(dnicliente);
    if (BuscandoSiExistrArchivo.good())
    {
        BuscandoSiExistrArchivo.close();
        return true;
    }
    else
    {
        BuscandoSiExistrArchivo.close();
        return false;
    }
}


void VerCarritoPorCliente(){
    cout<<"\t\t\t\t\t\t                   VER CARRITO DE COMPRA"<<endl;
    cout<<"\t\t\t\t\t\t                   ---------------------"<<endl;
    string dnicliente;
    fflush(stdin);

    cout<<endl;
    cout<<"\t\t\t\t\tINSERTE SU DNI: ";getline(cin,dnicliente);
    cout<<endl;
    if (ExistenciaDeUnCliente(dnicliente)==true)
    {
        ifstream Boleta;
        Boleta.open(dnicliente+".txt");
        string texto;
        if (Boleta.good())
        {
                cout<<"\t\t\t\t\t\t\tBOLETA DE COMPRA DEL USUARIO CON DNI "<<dnicliente<<endl;
            while (!Boleta.eof())
            {
                getline(Boleta,texto);
                cout<<"\t\t\t\t\t\t\t"<<texto<<endl;
            }

        }
    }
    else
    {
        cout<<endl;
        cout<<"\t\t\t\t\t\t\t\tUSTED NO ESTA REGISTRADO , REGISTRETESE"<<endl;
        cout<<endl;
    }


}


//funciones del menu

void LogueComoAdmin(){
    char usuario[10];
    char clave[12];
    int intento =0; //NUMERO DE INTENTOS
    char caracter;  //CARACTER POR CARACTER
    int i=0;        //CONTADOR

    do
    {
        system("cls");
        cout<<endl;
        cout<<"\t\t\t\tINICIO DE SESION COMO ADMINISTRADOR DE LA TIENDA"<<endl;
        cout<<"\t\t\t\t------------------------------------------------"<<endl;
        fflush(stdin);
        cout<<endl;
        cout<<"\t\t\t\tUSUARIO: ";cin.getline(usuario,10,'\n');
        fflush(stdin);
        cout<<"\t\t\t\t  CLAVE: ";
        i=0;

        while (caracter=getch())
        {
            if (caracter==13)
            {
                clave[i]='\0';
                break;
            }
            else
            {   if (caracter==8)
                {
                    if (i>0)
                    {
                        i--;
                        cout<<"\b \b";
                    }
                }
                else
                {
                    cout<<"*";
                    clave[i]=caracter;
                    i++;
                }
            }
        }

        cout<<endl;
        fflush(stdin);

        if (strcmp(USUARIO,usuario)==0 && strcmp(CONTRASENIA,clave)==0)
        {
            break;
        }

        else
        {
            cout<<endl;
            cout<<"\t\t\t\t\tUSUARIO Y/O CLAVE INCORRECTA"<<endl;
            intento++;
            cout<<"\t\t\t\t\t"<<"\tINTENTO: "<<(intento)<<"/3"<<endl;
            cout<<"\t\t\t\t\t"<<system("pause");
        }
        cout<<"\t\t\t\t";

    } while (intento<3);



    if (strcmp(USUARIO,usuario)==0 && strcmp(CONTRASENIA,clave)==0)
    {   int x=40, y=7;
        string nom={"CARGANDO DATOS"};
        cargando(x,y,nom);
        cout<<endl;
        cout<<"\t\t\t\t\t\tCLAVE CORRECTA"<<endl;
        cout<<"\t\t\t\t\t\t--------------"<<endl;
        cout<<"\t\t\t\t\t";system("pause");
        cout<<endl;
        system("cls");
        MenuDeAdmin();
    }

    else
    {
        cout<<endl;
        cout<<"\t\t\t\t  SOBREPASO EL NUMERO MAXIMO DE INTENTOS PERMITIDOS"<<endl;
    }
}


void MenuDeAdmin(){

    int opcion; // OPCIONES A DONDE QUIERA ENTRAR EL ADMINISTRADOR
    do
    {
        system("cls");
        cout<<"\t\t\t\t\tPANEL DE CONTROL DEL ADMINISTRADOR DE LA TIENDA"<<endl;
        cout<<"\t\t\t\t\t-----------------------------------------------"<<endl;
        cout<<endl;
        cout<<"\t\t[1]  REGISTRAR CELULAR"<<endl;
        cout<<"\t\t[2]  VER CELULARES REGISTRADOS"<<endl;
        cout<<"\t\t[3]  ACTUALIZAR PRECIO Y/O STOCK"<<endl;
        cout<<"\t\t[4]  BORRAR UN CELULAR"<<endl;
        cout<<"\t\t[5]  SALIR"<<endl;
        cout<<"\t\t     INSERTAR OPCION: ";
        opcion=ValidarDatosEnteros();
        switch (opcion)
        {
            case 1:
                system("cls");
                GuardarCelular();
                system("pause");
                break;
            case 2:
                system("cls");
                MostrarTodosLosCelulares();
                cout<<endl;
                cout<<"\t\t\t\t\t\t";system("pause");
                break;
            case 3:
                    char codigo[25];

                    do
                    {
                        system("cls");
                        MostrarTodosLosCelulares();
                        fflush(stdin);
                        cout<<endl;
                        cout<<"\t\t\t\t\t\tCODIGO DEL CELULAR A MODIFICAR : ";cin.getline(codigo,25,'\n');

                            if (ExistenciaDeUnCelular(codigo)==false)
                            {
                                cout<<"\t\t\t\t\t\t  CODIGO INVALIDO VUELVA A INTENTAR"<<endl;
                                cout<<"\t\t\t\t\t\t";system("pause");
                            }

                    } while (ExistenciaDeUnCelular(codigo)==false);

                    fflush(stdin);
                    ModificarUnCelular(codigo);
                    cout<<"\t\t\t\t\t\t",system("pause");
                    break;
            case 4:

                    char codigo2[25];
                    do
                    {

                        system("cls");
                        MostrarTodosLosCelulares();
                        fflush(stdin);
                        cout<<endl;
                        cout<<"\t\t\t\t\t\tCODIGO DEL PRODUCTO QUE DESEA ELIMINAR: ";cin.getline(codigo2,25,'\n');

                            if ( ExistenciaDeUnCelular(codigo2)==false)
                            {
                                cout<<"\t\t\t\t\t\t  CODIGO INVALIDO VUELVA A INTENTAR"<<endl;
                                cout<<"\t\t\t\t\t\t";system("pause");
                            }

                    } while (ExistenciaDeUnCelular(codigo2)==false);

                    fflush(stdin);
                    Eliminar(codigo2);
                    cout<<"\t\t\t\t\t\t",system("pause");
                    break;

            case 5:
                 cout<<"\t\t\t\tUSTED HA SALIDO DEL MENU"<<endl;
                 break;
            default:
                cout<<"\t\t\t\tDIGITE UNA OPCION VALIDA"<<endl;
                break;
        }
    } while (opcion!=5);
}


void cliente(){
    int opcion;
    do
    {
        system("cls");
        cout<<endl;
        cout<<"\t\t\t\t\t\tINTERFAZ PARA CLIENTES DE LA TIENDA CJURO-CAGON"<<endl;
        cout<<"\t\t\t\t\t\t-----------------------------------------------"<<endl;
        cout<<"\n";
        cout<<"\t\t[1]  REGISTRARME COMO CLIENTE"<<endl;
        cout<<"\t\t[2]  AGREGAR CELULAR AL CARRITO"<<endl;
        cout<<"\t\t[3]  VER MI CARRITO"<<endl;
        cout<<"\t\t[4]  SALIR"<<endl;
        cout<<endl;
        cout<<"\t\t     INSERTAR OPCION: ";
        opcion=ValidarDatosEnteros();
        switch (opcion)
        {
            case 1:
                system("cls");
                GuardarCliente();
                cout<<"\t\t\t\t\t\t\t";system("pause");
                break;
            case 2:
                system("cls");
                AgregarCelularAlCarrito();
                system("pause");
                break;
            case 3:
                system("cls");
                VerCarritoPorCliente();
                system("pause");
                break;
            case 4:
                cout<<"\t\t\t\tUSTED HA SALIDO DEL MENU"<<endl;
                break;
            default:
                cout<<endl;
                cout<<"\t\t\t\t\t\tDIGITE UNA OPCION VALIDA"<<endl;
                cout<<"\t\t\t\t\t\t";system("pause");
                break;
        }
    } while (opcion!=4);
}

void MenuPrincipal(){ // FUNCION PRINCIPAL DEL MENU
    int op;
    do
    {
        system("cls");
        cout<<"\n\n"<<endl;
        cout<<"\t\t\t\t\tTIENDA DE CELULARES DUNNED"<<endl;
        cout<<"\t\t\t\t\t---------------------------"<<endl;
        cout<<'\n';
        cout<<"\t\t[1]  INICIAR SESION COMO ADMINISTRADOR DE LA TIENDA"<<endl;
        cout<<"\t\t[2]  INICIAR COMO CLIENTE DE LA TIENDA"<<endl;
        cout<<"\t\t[3]  SALIR DE LA APLICACION"<<endl;
        cout<<endl;
        cout<<"\t\t     ELEGIR UNA OPCION : ";
        op=ValidarDatosEnteros();
        cout<<"\n";
        switch (op)
        {
            case 1:
                system("cls");
                LogueComoAdmin();
                system("pause");
                break;

            case 2:
                system("cls");
                cliente();
                system("pause");
                break;

            case 3:
                system("cls");
                system("pause");
                break;

            default:
                cout<<"\t\t\t\tDIGITE UNA OPCION VALIDA"<<endl;
                cout<<"\t\t\t\t";
                system("pause");
                break;
        }


    } while (op!=3);

}


//Validar Datos Enteros
int ValidarDatosEnteros(){
    int x;
    char A[30];
    cin.getline(A,30);
        if(atoi(A)==0){
            return -1;
        }
        else{
            x=atoi(A);
        }
    return x;
}

//Validar Datos Flotantes
float ValidarDatosFlotantes(){
    float  x;
    char A[30];
    cin.getline(A,30);
        if(atof(A)==0){
            return -1;
        }
        else{
            x=atof(A);
        }
    return x;
}
void logodeventa(){
int x=30, y=1;
gotoxy(x,y);  cout<<"          ||    ||  ||||||| ||||    || |||||||| |||||||   "<<endl;
gotoxy(x,y+1);cout<<"          ||    ||  ||      || ||   ||    ||    ||   ||   "<<endl;
gotoxy(x,y+2);cout<<"          ||    ||  ||||||| ||  ||  ||    ||    ||   ||   "<<endl;
gotoxy(x,y+3);cout<<"           ||  ||   ||      ||   || ||    ||    |||||||   "<<endl;
gotoxy(x,y+4);cout<<"             ||     ||||||| ||    ||||    ||    ||   ||   "<<endl;
gotoxy(x+20,y+6); cout<<"  |||||||      ||||||| "<<endl;
gotoxy(x+20,y+7); cout<<"  ||     ||    ||      "<<endl;
gotoxy(x+20,y+8); cout<<"  ||      ||   ||||||| "<<endl;
gotoxy(x+20,y+9); cout<<"  ||     ||    ||      "<<endl;
gotoxy(x+20,y+10);cout<<"  |||||||      ||||||| "<<endl;
gotoxy(x,y+12); cout<<"  |||||||  |||||| ||     ||    || ||     ||||||| ||||||| |||||| |||||||         "<<endl;
gotoxy(x,y+13); cout<<"  ||       ||     ||     ||    || ||     ||   || ||   || ||     ||          "<<endl;
gotoxy(x,y+14); cout<<"  ||       |||||| ||     ||    || ||     ||   || ||||||  |||||| |||||||         "<<endl;
gotoxy(x,y+15); cout<<"  ||       ||     ||     ||    || ||     ||||||| || ||   ||          ||  "<<endl;
gotoxy(x,y+16); cout<<"  |||||||  |||||| |||||| |||||||| |||||| ||   || ||   || |||||| |||||||    "<<endl;
}
void logocelular(){
char a,b,c,d, e, f, g, h;
char i, j, k, l, m, n, o,p, r;
a=char(218); //┌
b=char(196); // ─
c=char(179); // │
d=char(192); // └
e=char(32); // ESPACIO
f=char(191); // ┐
g=char(217); // ┘
h=char(219); // █
i=char(201); //╔
j=char(204); //╠
k=char(186); //║
l=char(187); // ╗
m=char(188); //╝
n=char(205); //═
o=char(185); //╣
p=char(200); //╚
r=char(223); //▀
int x=3, y=50;
cout<<e<<i<<n<<n<<n<<n<<n<<n<<n<<n<<n<<n<<n<<n<<n<<n<<n<<n<<n<<n<<l<<endl;
cout<<e<<k<<e<<r<<r<<r<<e<<e<<e<<e<<e<<e<<e<<e<<e<<e<<e<<e<<e<<e<<k<<endl;
for(int i=0; i<5; i++){
cout<<e<<k<<e<<e<<e<<e<<e<<e<<e<<e<<e<<e<<e<<e<<e<<e<<e<<e<<e<<e<<k<<endl;
}
cout<<e<<k<<e<<e<<e<<e<<e<<e<<"GOOGLE"<<e<<e<<e<<e<<e<<e<<k<<endl;
cout<<e<<k<<e<<e<<e<<a<<b<<b<<b<<b<<b<<b<<b<<b<<b<<b<<f<<e<<e<<e<<k<<endl;
cout<<e<<k<<e<<e<<e<<d<<b<<b<<b<<b<<b<<b<<b<<b<<b<<b<<g<<e<<e<<e<<k<<endl;
for(int i=0; i<6; i++){
cout<<e<<k<<e<<e<<e<<e<<e<<e<<e<<e<<e<<e<<e<<e<<e<<e<<e<<e<<e<<e<<k<<endl;
}
cout<<e<<j<<n<<n<<n<<n<<n<<n<<n<<n<<n<<n<<n<<n<<n<<n<<n<<n<<n<<n<<o<<endl;
for(int i=0; i<2; i++){
cout<<e<<k<<h<<h<<h<<h<<h<<h<<h<<h<<h<<h<<h<<h<<h<<h<<h<<h<<h<<h<<k<<endl;
}
cout<<e<<p<<n<<n<<n<<n<<n<<n<<n<<n<<n<<n<<n<<n<<n<<n<<n<<n<<n<<n<<m<<endl;
}
void interfaz(){
system("cls");
logocelular();
logodeventa();
gotoxy(15,25);
system("pause");
}
void cargando(int x, int y, string nom){
for(int i=1; i<=25; i++){
gotoxy(x,y);
cout<<" "<<nom<<" "<<4*i<<" % ";
gotoxy(i+x,y+2);
cout<<char(219);
Sleep(50);
}
cout<<endl;
}
