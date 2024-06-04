# Github

Iniciar el repositorio local
```
git init
```

Establecer el usuario
```
git config --global user.name "Tu Nombre"
git config --global user.email "tu@email.com"
```

Asociar la URL del repositorio
```
git remote set-url origin https://github.com/lcismondi/desktop-gadget.git 
```

Consultar la URL asociada
```
git remote -v
```

Descargar la rama y fusionarla directamente
```
git pull origin nombre-de-la-rama
```

Gestionar nuevas versiones
```
git status
git add .      //Agrega los archivos nuevos y modificados al área de "preparación" (staged)
git commit -m "Comentario"      //Crea una nueva versión en el ambiente local
git branch -M main              //Renombra la rama (forzado)
//Envía los commits de la rama "main" al repositorio remoto "origin".
//La opción -u establece "origin"" como la referencia predeterminada para futuros push y pull de la rama "main".
git push -u origin main
```
