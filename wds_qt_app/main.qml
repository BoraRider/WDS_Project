import QtQuick 2.12
import QtQuick.Window 2.12
import Qt3D.Core 2.0
import Qt3D.Input 2.0
import Qt3D.Extras 2.9
import QtQuick.Scene3D 2.0
import Qt3D.Render 2.15


Rectangle {
    id: root

      Scene3D {
        anchors.fill: root
        focus: true

        Entity {
        id: sceneID

        property var x: 0
        property var y: 180
        property var z: -90
        property var trans: Qt.vector3d(0, 0, 0)
        property var elementScale: 0.5


        components: [
            RenderSettings {
                activeFrameGraph: ForwardRenderer {
                    camera: camera
                }
            },

            InputSettings { }
        ]

        Camera {
            id: camera
            projectionType: CameraLens.PerspectiveProjection
            fieldOfView: 90
            nearPlane : 0.1
            farPlane : 1000.0
            position: Qt.vector3d( -12.0, -3.0, 0.0 )
            upVector: Qt.vector3d( 0.0, 0.0, 0.0 )
            viewCenter: Qt.vector3d( 0.0, 0.0, 0.0 )
        }

        FirstPersonCameraController { camera: camera }

        Entity {
            id: lights
            Entity {
                components: [
                    DirectionalLight {
                        worldDirection: Qt.vector3d(0.3, -3.0, 0.0).normalized();
                        color: "#fbf9ce"
                        intensity: 0.5
                    }
                ]
            }
        }

        Entity {
            id: hand_core
            components: [
                Mesh {
                    source: "qrc:/new/prefix1/models/hand_core.obj"
                },
                PhongMaterial {
                    ambient: "#e81109"
                    diffuse: "#e81109"
                },

                Transform {
                    //id: transform
                    translation: sceneID.trans
                    rotationX: sceneID.x + gyro_x
                    rotationY: sceneID.y + gyro_y
                    rotationZ: sceneID.z + gyro_z
                    scale: sceneID.elementScale
                }
            ]
        }
        Entity {
            id: thumb
            components: [
                Mesh {
                    source: "qrc:/new/prefix1/models/thumb.obj"
                },
                PhongMaterial {
                    ambient: "#fcad03"
                    diffuse: "#fcad03"
                },
                Transform {
                    translation: sceneID.trans
                    rotationX: sceneID.x + gyro_x - f1
                    rotationY: sceneID.y + gyro_y
                    rotationZ: sceneID.z + gyro_z
                    scale: sceneID.elementScale
                }
            ]
        }
        Entity {
            id: finger1
            components: [
                Mesh {
                    source: "qrc:/new/prefix1/models/finger1.obj"
                },
                PhongMaterial {
                    ambient: "#2dd10d"
                    diffuse: "#2dd10d"
                },
                Transform {
                    translation: sceneID.trans
                    rotationX: sceneID.x + gyro_x
                    rotationY: sceneID.y + gyro_y
                    rotationZ: sceneID.z + gyro_z - f2
                    scale: sceneID.elementScale
                }
            ]
        }
        Entity {
            id: finger2
            components: [
                Mesh {
                    source: "qrc:/new/prefix1/models/finger2.obj"
                },
                PhongMaterial {
                    ambient: "#09ede2"
                    diffuse: "#09ede2"
                },
                Transform {
                    translation: sceneID.trans
                    rotationX: sceneID.x + gyro_x
                    rotationY: sceneID.y + gyro_y
                    rotationZ: sceneID.z + gyro_z - f3
                    scale: sceneID.elementScale
                }
            ]
        }
        Entity {
            id: finger3
            components: [
                Mesh {
                    source: "qrc:/new/prefix1/models/finger3.obj"
                },
                PhongMaterial {
                    ambient: "#092bed"
                    diffuse: "#092bed"
                },
                Transform {
                    translation: sceneID.trans
                    rotationX: sceneID.x + gyro_x
                    rotationY: sceneID.y + gyro_y
                    rotationZ: sceneID.z + gyro_z - f4
                    scale: sceneID.elementScale
                }
            ]
        }
        Entity {
            id: finger4
            components: [
                Mesh {
                    source: "qrc:/new/prefix1/models/finger4.obj"
                },
                PhongMaterial {
                    ambient: "#bf09ed"
                    diffuse: "#bf09ed"
                },
                Transform {
                    translation: sceneID.trans
                    rotationX: sceneID.x + gyro_x
                    rotationY: sceneID.y + gyro_y
                    rotationZ: sceneID.z + gyro_z - f5
                    scale: sceneID.elementScale
                }
            ]
        }
      }
    }
}
