#include <vtkActor.h>
#include <vtkNamedColors.h>
#include <vtkPolyData.h>
#include <vtkPolyDataMapper.h>
#include <vtkProperty.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkRenderer.h>
#include <vtkSmartPointer.h>
#include <vtkSphereSource.h>
#include <vtkCubeSource.h>
#include <vtkCamera.h>
#include <vtkPNGWriter.h>
#include <vtkMPIController.h>
#include <vtkMPICommunicator.h>
#include <vtkMPIImageFilter.h>

int main(int, char* [])
{
	// Initialize MPI TODO
	vtkMPIController* controller = vtkMPIController::New();
	controller->Initialize(&argc, &argv, 1);

	vtkNew<vtkNamedColors> colors;

	//Sphere-----------------------------------------------
	// Create a sphere
	vtkNew<vtkSphereSource> sphereSource;
	sphereSource->SetCenter(0.0, 0.0, 0.0);
	sphereSource->SetRadius(1.0);
	// Make the surface smooth.
	sphereSource->SetPhiResolution(10);
	sphereSource->SetThetaResolution(10);

	vtkNew<vtkPolyDataMapper> mapper_sphere;
	mapper_sphere->SetInputConnection(sphereSource->GetOutputPort());

	vtkNew<vtkActor> actor_sphere;
	actor_sphere->SetMapper(mapper_sphere);
	actor_sphere->GetProperty()->SetColor(colors->GetColor3d("DarkGreen").GetData());
	//----------------------------------------------------------
	vtkNew<vtkCubeSource> cubeSource;
	cubeSource->SetXLength(1.0);
	cubeSource->SetYLength(1.0);
	cubeSource->SetZLength(1.0);
	cubeSource->SetCenter(4.0, 0.0, 0.0);
	vtkNew<vtkPolyDataMapper> mapper_cube;
	mapper_cube->SetInputConnection(cubeSource->GetOutputPort());

	vtkNew<vtkActor> actor_cube;
	actor_cube->SetMapper(mapper_cube);
	actor_cube->GetProperty()->SetColor(colors->GetColor3d("DarkBlue").GetData());
	//-------------------------------------------------------------
	vtkNew<vtkRenderer> renderer;
	vtkNew<vtkRenderWindow> renderWindow;
	renderWindow->SetWindowName("Sphere&Cube");
	renderWindow->AddRenderer(renderer);
	vtkNew<vtkRenderWindowInteractor> renderWindowInteractor;
	renderWindowInteractor->SetRenderWindow(renderWindow);

	renderer->AddActor(actor_sphere);
	renderer->AddActor(actor_cube);
	renderer->SetBackground(colors->GetColor3d("DarkRed").GetData());

	renderWindow->Render();
	renderWindowInteractor->Start();

	return EXIT_SUCCESS;
}