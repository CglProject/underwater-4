
uniform mediump mat4 ViewMatrix;
uniform mediump mat4 ModelMatrix;
uniform mediump mat4 ProjectionMatrix;

uniform mediump mat3 NormalMatrix;

uniform mediump vec4 LightPos;
uniform mediump vec4 EyePos;

uniform lowp vec3 Ka;   // ambient material coefficient
uniform lowp vec3 Kd;   // diffuse material coefficient
uniform lowp vec3 Ks;   // specular material coefficient

uniform mediump float Ns;   // specular material exponent (shininess)

uniform lowp vec3 Ia;   // ambient light intensity
uniform lowp vec3 Id;   // diffuse light intensity
uniform lowp vec3 Is;   // specular light intensity

uniform sampler2D DiffuseMap;
uniform sampler2D SpecularMap;
uniform sampler2D NormalMap;

varying lowp vec4 ambientVarying;
varying lowp vec4 diffuseVarying;
varying lowp vec4 specularVarying;
varying lowp vec4 texCoordVarying;

varying mediump vec4 posVarying;        // pos in world space
varying mediump vec3 normalVarying;     // normal in world space
varying mediump vec3 tangentVarying;    // tangent in world space

void main()
{
    lowp vec4 ambientResult = vec4(Ka * Ia, 1.0);
    
    mediump vec4 pos = posVarying;
    mediump vec3 n = normalize(normalVarying);
    
    // Calculate tbn matrix, ensure it's orthonormal
    // =============================================
    
    // Vectors for Gram-Schmidt orthonormalization
    // -------------------------------------------
    
    // mediump vec3 w1 = n;
    // mediump vec3 w2 = tangentVarying;
    mediump vec3 w3 = cross(n, tangentVarying);
    
    // Do Gram-Schmidt orthonormalization
    // ----------------------------------
    
    // mediump vec3 v1 = normalize(w1) // = n
    mediump vec3 t = normalize(tangentVarying - dot(n, tangentVarying) * n);
    mediump vec3 b = normalize(w3 - dot(n, w3) * n - dot(t, w3) * t);
    
    // Create TBN matrix
    // -----------------
    
    mediump mat3 tbn = mat3(t, b, n);
    
    // Read and correctly transform normals from normal map...
    // =======================================================
    
    mediump vec3 normalFromMap = texture2D(NormalMap, texCoordVarying.st).xyz;
    normalFromMap = normalize(2.0 * (normalFromMap - 0.5)); // Transform color value to vector
    mediump vec3 transformedNormalFromMap = normalize(tbn * normalFromMap); // Transform normal using TBN matrix
    
    // ... then use them for lighting
    // ==============================
    
    mediump vec3 l = normalize(LightPos - pos).xyz;
    
    lowp float intensity = dot(transformedNormalFromMap, l);
    lowp vec3 diffuse = Kd * clamp(intensity, 0.0, 1.0) * Id;
    lowp vec4 diffuseResult = vec4(clamp(diffuse, 0.0, 1.0), 1.0);
    
    // If vertex is lit, calculate specular term in view space using the Blinn-Phong model
    lowp vec4 specularResult = vec4(0.0);
    if (intensity > 0.0)
    {
        mediump vec3 eyeVec = normalize(EyePos.xyz - pos.xyz);
        mediump vec3 h = normalize(l + eyeVec);
        
        mediump float specIntensity = pow(max(dot(h,transformedNormalFromMap), 0.0), Ns);
        mediump vec3 specular = Ks * clamp(specIntensity, 0.0, 1.0) * Is;
        
        // Use the specular map. According to the slides I just have to "Multiply [specular] result by value
        // from specular map". I am doing it here so the `clamp` still has effect.
        specular = specular * texture2D(SpecularMap, texCoordVarying.st).x;
        
        specularResult = vec4(clamp(specular, 0.0, 1.0), 1.0);
    }

    lowp vec4 color = texture2D(DiffuseMap, texCoordVarying.st);
    gl_FragColor = (ambientResult + diffuseResult) * color + specularResult;
    // gl_FragColor = (ambientResult + diffuseResult + specularResult) * color;
    // gl_FragColor = vec4(1.0, 0.0, 0.0, 1.0);
    // gl_FragColor = color;
}
