import QtQuick 2.12
import QtQuick.Window 2.12
import Qt3D.Core 2.0
import Qt3D.Render 2.0
import Qt3D.Input 2.0
import Qt3D.Extras 2.9


Entity {
    id: root
    objectName: "root"




    // Use the renderer configuration specified in ForwardRenderer.qml
    // and render from the mainCamera
    components: [
        RenderSettings {
            activeFrameGraph: ForwardRenderer {
                camera: mainCamera
            }
        },
        // Event Source will be set by the Qt3DQuickWindow
        InputSettings { }
    ]

    BasicCam {
        id: mainCamera
        position: Qt.vector3d( -5.17253, 2.95727, 6.65948 )
        viewCenter: Qt.vector3d( 6.73978, -2.50545, -10.6525 )
    }

    FirstPersonCameraController { camera: mainCamera }

    Lights { }

    Entity {
        components: [
            PointLight {
                enabled: parent.enabled
                color: "black"
                intensity: 0
            }
        ]
    }
    Entity {
        id: floor

        components: [
            Mesh {
                source: "models/pane.obj"
            },
            MetalRoughMaterial {
                //id: material
                baseColor: Qt.rgba( 0.1, 0.4, 0.8, 1.0 )
                metalness: 0.0
                roughness: 0.9
            },

            Transform {
                //id: transform
                //translation: Qt.vector3d(root.x, root.y, root.z)
                //rotationX: theta
                //rotationY: phi
                scale: root.scale
            }
        ]
    }

    HandCore {
            id: reka
            y: 1.5
            z: -2
            scale: 0.5
            SequentialAnimation {
                loops: Animation.Infinite
                running: true
                NumberAnimation {
                    target: reka
                    property: "alpha"
                    from: 0; to: 90
                    duration: 1000
                }
                NumberAnimation {
                    target: reka
                    property: "alpha"
                    from: 90; to: 0
                    duration: 1000
                }
            }
        }

    Finger1 {
            id: f1
            y: 1.5
            z: -2
            scale: 0.5

            ParallelAnimation{
                loops: Animation.Infinite
                running: true

                SequentialAnimation {
                    loops: Animation.Infinite
                    running: true

                    NumberAnimation {
                        target: f1
                        property: "alpha"
                        from: 0; to: 90
                        duration: 1000
                    }
                    NumberAnimation {
                        target: f1
                        property: "alpha"
                        from: 90; to: 0
                        duration: 1000
                    }
                }

                SequentialAnimation {
                    loops: Animation.Infinite
                    running: true

                    NumberAnimation {
                        target: f1
                        property: "beta"
                        from: 0; to: 90
                        duration: 1000
                    }
                    NumberAnimation {
                        target: f1
                        property: "beat"
                        from: 90; to: 0
                        duration: 1000
                    }
                }
            }


        }



}

