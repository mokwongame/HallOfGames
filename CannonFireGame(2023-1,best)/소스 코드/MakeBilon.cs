using UnityEngine;

public class MakeBilon : MonoBehaviour
{
    public GameObject bilon;
    public float startTime = 8.0f;
    float makeRate = 10.0f;
    public float ymin = -3.0f;
    public float ymax = 9.0f;
    // Start is called before the first frame update
    void Start()
    {
        InvokeRepeating("makeBilonObj", startTime, makeRate);
    }

    // Update is called once per frame
    void Update()
    {

    }

    void makeBilonObj()
    {
        Vector3 vec3;
        vec3.x = 33;
        vec3.y = Random.Range(ymin, ymax);
        vec3.z = 0.0f;
        Instantiate(bilon, vec3, transform.rotation);
    }


}